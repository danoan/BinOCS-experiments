#include "FlowControl.h"

using namespace MostExternContribution;

FlowControl::DigitalSet FlowControl::resolveShape(Shape shape,double gridStep)
{
    int radius=20;
    if(shape==Shape::Triangle) return DIPaCUS::Shapes::triangle(gridStep,0,0,radius);
    else if(shape==Shape::Square) return DIPaCUS::Shapes::square(gridStep,0,0,radius);
    else if(shape==Shape::Pentagon) return DIPaCUS::Shapes::NGon(gridStep,0,0,radius,5);
    else if(shape==Shape::Heptagon) return DIPaCUS::Shapes::NGon(gridStep,0,0,radius,7);
    else if(shape==Shape::Ball) return DIPaCUS::Shapes::ball(gridStep,0,0,radius);
    else if(shape==Shape::Flower) return DIPaCUS::Shapes::flower(gridStep,0,0,radius,radius/2.0,2);
    else if(shape==Shape::Ellipse) return DIPaCUS::Shapes::ellipse(gridStep,0,0,radius,radius/2);
}

FlowControl::FlowControl(const BCFlowInput& bcFlowInput,
                         Shape shape,
                         double gridStep,
                         const std::string& outputFolder,
                         std::ostream& osLog)
{
    boost::filesystem::create_directories(outputFolder);

    DigitalSet ds = resolveShape(shape,gridStep);
    shapeFlow( ds,bcFlowInput,outputFolder,osLog );
}

void FlowControl::createMostExternContributionFigure(const BCAInput& bcaInput, const std::string& outputPath)
{
    ODRInterface& odrFactory = ODRPool::get(bcaInput.odrConfigInput);

    ODRModel odr = odrFactory.createODR(ODRModel::OptimizationMode::OM_OriginalBoundary,
                                       ODRModel::ApplicationMode::AM_AroundBoundary,
                                       bcaInput.bcConfigInput.radius,
                                       bcaInput.imageDataInput.inputDS,
                                       bcaInput.odrConfigInput.optInApplicationRegion);

    const DigitalSet& appRegion = odr.applicationRegion;
    const DigitalSet& optRegion = odr.optRegion;

    std::map<DGtal::Z2i::Point,int> hits;
    for(auto it=optRegion.begin();it!=optRegion.end();++it) hits[*it] = 0;


    DigitalSet temp(optRegion.domain());
    DIPaCUS::Misc::DigitalBallIntersection DBI(bcaInput.bcConfigInput.radius,odr.optRegion);
    for(auto it=appRegion.begin();it!=appRegion.end();++it)
    {
        DBI(temp,*it);
        for(auto ito=temp.begin();ito!=temp.end();++ito)
        {
            hits[*ito] += 1;
        }
        temp.clear();
    }

    DGtal::Board2D board;
    std::string specificStyle = odr.original.className() + "/Paving";
    board << DGtal::SetMode(odr.original.className(),"Paving");

    board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Blue, DGtal::Color::Blue));
    board << odr.trustFRG;

    board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Green, DGtal::Color::Green));
    board << odr.optRegion;

    board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Red, DGtal::Color::Red));
    board << odr.applicationRegion;

    DGtal::Z2i::Point pt = *appRegion.begin();
    DigitalSet ballDS = DIPaCUS::Shapes::ball(1.0,pt(0),pt(1),bcaInput.bcConfigInput.radius);
    board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Silver, DGtal::Color::Yellow));
    board << ballDS;


    int min,max;
    min=hits[*optRegion.begin()];
    max=hits[*optRegion.begin()];
    for(auto it=optRegion.begin();it!=optRegion.end();++it)
    {
        if(hits[*it]<min) min = hits[*it];
        if(hits[*it]>max) max = hits[*it];
    }

    std::cout << max << " :: " << min << std::endl;

    DGtal::GradientColorMap<double,
            DGtal::ColorGradientPreset::CMAP_GRAYSCALE> cmap_jet(min,max);

    board << DGtal::SetMode( optRegion.begin()->className(), "Paving" );
    std::string pointStyle = optRegion.begin()->className() + "/Paving";

    for(auto it=optRegion.begin();it!=optRegion.end();++it)
    {
        board << DGtal::CustomStyle(pointStyle, new DGtal::CustomColors(DGtal::Color::Black,
                                                                           cmap_jet( hits[*it] )));
        board << *it;
    }


    board.saveSVG(outputPath.c_str(),200,200,10);
}


FlowControl::BCAOutput FlowControl::boundaryCorrection(const BCFlowInput& bcFlowInput,
                                                       const cv::Mat& currentImage,
                                                       const std::string& outputPath,
                                                       Point& translation)
{
    MockDistribution frDistr;
    MockDistribution bkDistr;

    ImageDataInput imageDataInput(frDistr,
                                  bkDistr,
                                  currentImage,
                                  currentImage);


    BCAInput bcaInput(bcFlowInput.bcInput,
                      imageDataInput,
                      bcFlowInput.odrConfigInput,
                      bcFlowInput.flowProfile);

    createMostExternContributionFigure(bcaInput,outputPath);

    BCAOutput bcaOutput(bcaInput);

    BTools::Core::BCApplication BCA(bcaOutput,
                                    bcaInput,
                                    1,
                                    false);

    translation = imageDataInput.translation;

    return bcaOutput;
}

FlowControl::DigitalSet FlowControl::correctTranslation(const BCAOutput::EnergySolution& solution,
                                                        const cv::Mat& currentImage,
                                                        const Point& translation)
{
    DigitalSet translatedBackDS( Domain( Point(0,0),
                                         Point(currentImage.cols-1,currentImage.rows-1)
    ) );

    for (auto it = solution.outputDS.begin(); it != solution.outputDS.end(); ++it)
    {
        translatedBackDS.insert(*it + translation );
    }

    return translatedBackDS;
}

void FlowControl::checkBounds(const DigitalSet& ds, const Domain& domain)
{
    Point lb,ub;
    ds.computeBoundingBox(lb,ub);

    if(lb(0) <= domain.lowerBound()(0)+1 ||
       lb(1) <= domain.lowerBound()(1)+1 ||
       ub(0) >= domain.upperBound()(0)-1 ||
       ub(1) >= domain.upperBound()(1)-1 )
    {
        throw std::runtime_error("Result image is too big.");
    }

    if(ds.size()<10) throw std::runtime_error("Result image is too small.");
}

void FlowControl::shapeFlow(const DigitalSet& _ds,
                            const BCFlowInput& bcFlowInput,
                            const std::string& outputFolder,
                            std::ostream& osLog)
{
    osLog << "Flow Start: " << bcFlowInput.inputName << "\n";
    osLog << "Iterations (" << bcFlowInput.maxIterations << "): ";


    boost::filesystem::create_directories(outputFolder);
    std::string currImagePath = outputFolder + "/" + BTools::Utils::nDigitsString(0,4) + ".pgm";
    std::ofstream os(outputFolder + "/" + bcFlowInput.inputName + ".txt");


    DigitalSet ds = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_ds,Point(20,20));
    Domain flowDomain = ds.domain();


    BTools::Utils::exportImageFromDigitalSet(ds,flowDomain,currImagePath);

    cv::Mat img = cv::imread(currImagePath,cv::IMREAD_COLOR);
    Domain solutionDomain(Point(0,0),Point(img.cols-1,img.rows-1));

    int i=1;
    try
    {
        do
        {
            osLog << "|";

            std::vector<IBCControlVisitor*> visitors;
            cv::Mat currentImage = cv::imread(currImagePath,cv::IMREAD_COLOR);


            Point translation;
            std::string contributionFigurePath = outputFolder + "/contrib_" + BTools::Utils::nDigitsString(i,4) + ".svg";
            BCAOutput bcaOutput = boundaryCorrection(bcFlowInput,currentImage,contributionFigurePath,translation);

            DigitalSet correctedSet = correctTranslation(bcaOutput.energySolution,currentImage,translation);
            checkBounds(correctedSet,flowDomain);


            currImagePath = outputFolder + "/" + BTools::Utils::nDigitsString(i,4) + ".pgm";
            BTools::Utils::exportImageFromDigitalSet(correctedSet,flowDomain,currImagePath);


            ++i;
        }while(i<bcFlowInput.maxIterations);
    }catch(std::exception ex)
    {
        osLog << ex.what() << "\n";
    }


}



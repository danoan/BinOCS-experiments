#include "BTools/reader/DCFReader.h"

using namespace BTools::Reader;

DCFReader::InputData DCFReader::defaultValues()
{
    InputData id;
    id.radius=3;
    id.iterations=10;

    id.neighborhood=InputData::ODRConfigInput::NeighborhoodType::FourNeighborhood;
    id.ld = InputData::ODRConfigInput::LevelDefinition::LD_CloserFromCenter;

    id.levels= id.radius;
    id.optRegionInApplication = false;

    id.fp = InputData::FlowProfile::DoubleStep;

    id.dtWeight = 0.5;
    id.sqWeight = 1.0;
    id.lgWeight = 0.2;

    id.optMethod = InputData::OptMethod::Improve;

    id.shape = Shape(ShapeType::Square);
    id.gridStep=1.0;

    id.excludeOptPointsFromAreaComputation = false;

    id.om = InputData::OptimizationMode::OM_CorrectConvexities;
    id.am = InputData::ApplicationMode::AM_OptimizationBoundary;

    return id;
}

void DCFReader::usage(char* argv[],const std::string& extraUsage)
{
    std::cerr << "Usage: " << argv[0] << "\n"
            "[-r Ball Radius default 3] \n"
            "[-i Max Iterations default 10] \n"
            "[-p FlowProfile single double single-opt double-opt single-inner double-inner default double] \n"
            "[-n Neighborhood 4 or 8 default: 4] \n"
            "[-d Use digital area default: false] \n"
            "[-l Computation levels. If negative, select LD_FartherFromCenter. Default: Ball radius] \n"
            "[-q Squared Curvature Term weight default: 1.0] \n"
            "[-t Data Term weight default: 1.0] \n"
            "[-g Length Term weight default: 1.0] \n"
            "[-m Opt method 'probe' 'improve' default: improve] \n"
            "[-o Include optimization region in the application region default: false \n"
            "[-S Shape (triangle square pentagon heptagon ball ellipse ball wave). Default: square\n"
            "[-h Grid step (default:1.0)]\n"
            "[-e Optimization mode (correct-convexities correct-concavities) (default:correct-convexities)]\n"
            "[-e Application mode (optimization-contour around-contour inner-contour outer-contour) (default:optimization-contour)]\n"
            "[-x Exclude opt points from computation area default: false] \n"
            <<  extraUsage << std::endl;
}

DCFReader::InputData DCFReader::readInput(int argc,char** argv,const std::string& extraUsage,DefaultValues df)
{
    InputData id = df();

    int opt;
    while( (opt=getopt(argc,argv,"r:i:p:n:dl:q:t:g:m:oS:h:e:f:x"))!=-1)
    {
        switch(opt)
        {
            case 'r':
                id.radius = atoi(optarg);
                break;
            case 'i':
                id.iterations= atoi(optarg);
                break;
            case 'p':
                if(strcmp(optarg,"single")==0 ) id.fp = InputData::FlowProfile::SingleStepConvexities;
                else if(strcmp(optarg,"double")==0 ) id.fp = InputData::FlowProfile::DoubleStep;
                else if(strcmp(optarg,"single-opt")==0 ) id.fp = InputData::FlowProfile::SingleStepOpt;
                else if(strcmp(optarg,"double-opt")==0 ) id.fp = InputData::FlowProfile::DoubleStepOpt;
                else if(strcmp(optarg,"single-inner")==0 ) id.fp = InputData::FlowProfile::SingleStepInner;
                else if(strcmp(optarg,"double-inner")==0 ) id.fp = InputData::FlowProfile::DoubleStepInner;
                else if(strcmp(optarg,"single-around-inner")==0 ) id.fp = InputData::FlowProfile::SingleStepAroundInner;
                else if(strcmp(optarg,"double-around-inner")==0 ) id.fp = InputData::FlowProfile::DoubleStepAroundInner;
                else std::runtime_error("Profile not recognized.");
                break;
            case 'n':
            {
                int n = atoi(optarg);
                if(n==4) id.neighborhood = InputData::ODRConfigInput::NeighborhoodType::FourNeighborhood;
                else if(n==8) id.neighborhood = InputData::ODRConfigInput::NeighborhoodType::EightNeighborhood;
                else throw std::runtime_error("Neighborhood value must be 4 or 8.");
                break;
            }
            case 'l':
                id.levels = atoi(optarg);
                if(id.levels<0) id.ld = InputData::ODRConfigInput::LevelDefinition::LD_FartherFromCenter;
                else id.ld = id.ld = InputData::ODRConfigInput::LevelDefinition::LD_CloserFromCenter;
                id.levels = abs(id.levels);
                break;
            case 'q':
                id.sqWeight = atof(optarg);
                break;
            case 't':
                id.dtWeight = atof(optarg);
                break;
            case 'g':
                id.lgWeight = atof(optarg);
                break;
            case 'm':
                if(strcmp(optarg,"probe")==0) id.optMethod = InputData::OptMethod::Probe;
                else if(strcmp(optarg,"improve")==0) id.optMethod = InputData::OptMethod::Improve;
                break;
            case 'o':
                id.optRegionInApplication = true;
                break;
            case 'S':
                if(strcmp(optarg,"triangle")==0) id.shape = Shape( ShapeType::Triangle);
                else if(strcmp(optarg,"square")==0) id.shape = Shape( ShapeType::Square );
                else if(strcmp(optarg,"pentagon")==0) id.shape = Shape( ShapeType::Pentagon);
                else if(strcmp(optarg,"heptagon")==0) id.shape = Shape( ShapeType::Heptagon);
                else if(strcmp(optarg,"ball")==0) id.shape = Shape( ShapeType::Ball);
                else if(strcmp(optarg,"ellipse")==0) id.shape = Shape( ShapeType::Ellipse);
                else if(strcmp(optarg,"flower")==0) id.shape = Shape( ShapeType::Flower);
                else if(strcmp(optarg,"wave")==0) id.shape = Shape( ShapeType::Wave);
                else id.shape = Shape(ShapeType::UserDefined,optarg);
                break;
            case 'h':
            {
                id.gridStep = std::atof(optarg);
                break;
            }
            case 'e':
            {
                if(strcmp("correct-convexities",optarg)==0) id.om = InputData::OptimizationMode::OM_CorrectConvexities;
                else if(strcmp("correct-concavities",optarg)==0) id.om = InputData::OptimizationMode::OM_CorrectConcavities;
                else throw std::runtime_error("Optimization Mode not recognized!");
                break;
            }
            case 'f':
            {
                if(strcmp("optimization-contour",optarg)==0) id.am = InputData::ApplicationMode::AM_OptimizationBoundary;
                else if(strcmp("around-countour",optarg)==0) id.am = InputData::ApplicationMode::AM_AroundBoundary;
                else if(strcmp("inner-countour",optarg)==0) id.am = InputData::ApplicationMode::AM_InternRange;
                else if(strcmp("outer-countour",optarg)==0) id.am = InputData::ApplicationMode::AM_ExternRange;
                else throw std::runtime_error("Optimization Mode not recognized!");
                break;
            }
            case 'x':
            {
                id.excludeOptPointsFromAreaComputation = true;
                break;
            }
            default:
            {
                usage(argv,extraUsage);
                exit(1);
            }
        }
    }

    return id;
}
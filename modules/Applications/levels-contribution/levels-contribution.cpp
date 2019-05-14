#include "InputReader.h"
#include "FlowControl.h"

using namespace MostExternContribution;

std::string resolveShapeName(Shape shape)
{
    if(shape==Shape::Triangle) return "triangle";
    else if(shape==Shape::Square) return "square";
    else if(shape==Shape::Pentagon) return "pentagon";
    else if(shape==Shape::Heptagon) return "heptagon";
    else if(shape==Shape::Ball) return "ball";
    else if(shape==Shape::Ellipse) return "ellipse";
    else if(shape==Shape::Flower) return "flower";
    else return "";
}

int main(int argc, char* argv[])
{
    InputReader::InputData id = InputReader::readInput(argc, argv);

    FlowControl::BCConfigInput bcInput(id.radius,
                                       0,
                                       1.0,
                                       0,
                                       0,
                                       id.excludeOptPointsFromAreaComputation,
                                       id.penalizationMode,
                                       InputReader::InputData::OptMethod::Improve);

    FlowControl::ODRConfigInput odrConfigInput(InputReader::InputData::ODRConfigInput::ApplicationCenter::AC_PIXEL,
                                               InputReader::InputData::ODRConfigInput::CountingMode::CM_PIXEL,
                                               InputReader::InputData::ODRConfigInput::SpaceMode::Pixel,
            id.radius,
            id.gridStep,
                    id.levels,
            id.ld,
            id.neighborhood,
            id.seType,
            id.opt);

    FlowControl::BCFlowInput bcFlowInput(resolveShapeName(id.shape),
                                         bcInput,
                                         odrConfigInput,
                                         InputReader::InputData::FlowProfile::DoubleStep,
                                         id.iterations);


    FlowControl flow(bcFlowInput,
                     id.shape,
                     id.gridStep,
                     id.outputFolder,
                     id.excludeOptPointsFromAreaComputation,
                     std::cerr);

    return 0;
}
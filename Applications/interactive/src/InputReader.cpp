#include "InputReader.h"

using namespace BTools::Application::Interactive;

void InputReader::readInput(InputData& id,
                            int argc,
                            char** argv)
{

    int opt;
    while( (opt=getopt(argc,argv,"c:d:l:b:i:"))!=-1)
    {
        switch(opt)
        {
            case 'c':
                id.bcFlowInput.bcInput.sqTermWeight = atof(optarg);
                break;
            case 'd':
                id.bcFlowInput.bcInput.dataTermWeight = atof(optarg);
                break;
            case 'l':
                id.bcFlowInput.bcInput.lengthTermWeight = atof(optarg);
                break;
            case 'b':
                id.bcFlowInput.bcInput.radius = atoi(optarg);
                break;
            case 'i':
                id.bcFlowInput.maxIterations = atoi(optarg);
                break;

            default:
                std::cerr << "Usage: %s IMAGE_PATH [-c Curvature Weight] "
                        "[-d Data Weight] [-l Length Weight] [-b Ball Radius] "
                        "[-i Max Iterations]" << std::endl;

        }
    }

    id.imageFilePath= argv[optind];
    id.bcFlowInput.bcInput.solverType = InputData::BCFlowInput::BCConfigInput::QPBOSolverType::Probe;
}
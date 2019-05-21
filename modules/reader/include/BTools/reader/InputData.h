#ifndef BTOOLS_INPUTDATA_H
#define BTOOLS_INPUTDATA_H


#include <unistd.h>

#include "BTools/core/model/input/BCConfigInput.h"
#include "BTools/core/model/input/ODRConfigInput.h"
#include "BTools/core/interface/IFlowProfile.h"

#include "Types.h"

namespace BTools
{
    namespace Reader
    {
        namespace DCFReader
        {
            struct InputData
            {
                typedef BTools::Core::ODRConfigInput ODRConfigInput;
                typedef BTools::Core::IFlowProfile::FlowProfile FlowProfile;
                typedef BTools::Core::BCConfigInput::QPBOSolverType OptMethod;

                typedef BTools::Core::BCConfigInput::PenalizationMode PenalizationMode;
                typedef BTools::Core::IFlowStepConfig::OptimizationMode OptimizationMode;
                typedef BTools::Core::IFlowStepConfig::ApplicationMode ApplicationMode;

                InputData(){};

                int radius;
                int iterations;

                ODRConfigInput::ApplicationCenter ac;
                ODRConfigInput::CountingMode cm;
                ODRConfigInput::SpaceMode  sm;
                ODRConfigInput::NeighborhoodType neighborhood;
                ODRConfigInput::LevelDefinition ld;
                ODRConfigInput::StructuringElementType seType;

                bool optRegionInApplication;
                int levels;

                FlowProfile fp;

                double sqWeight;
                double dtWeight;
                double lgWeight;
                double penalizationWeight;

                OptMethod optMethod;
                Shape shape;
                double gridStep;

                bool excludeOptPointsFromAreaComputation;
                PenalizationMode penalizationMode;

                bool repeatedImprovement;

                OptimizationMode om;
                ApplicationMode am;

            };
        }
    }
}

#endif //BTOOLS_INPUTDATA_H

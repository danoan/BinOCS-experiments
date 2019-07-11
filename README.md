## BTools
 BTools is a collection of applications developped for the 
 publication [1]. In this work, we describe a flow driven by 
 the minimization of the digital version of the elastica 
 energy. Its main applications are the **shape-flow** and 
 **boundary-correction**. The latter is an application of
 the flow in image segmentation.
 
 A friendly GUI is in stage of development.
 
 [1] Antunes, D., Lachaud, J.O., Talbot H.: Digital 
 curvature evolution model for image segmentation. In:
 Couprie, M., Cousty, J., Kenmochi, Y., Mustafa, N. (eds.) 
 Discrete Geometry for Computer Imagery. pp 15-26. Springer
 International Publishing, Cham (2019).
 
## Dependencies 

[libboost1.66.0-dev](https://www.boost.org/users/history/version_1_66_0.html)

[DGtal0.9](https://dgtal.org/download/)

[opencv-3.3.0](https://opencv.org/releases.html)

## How to use

Build instructions can be found in INSTALL.txt

This source contains five applications. For detailed description,
type: [application] -?

### seed-selector

Tool to select foreground, background and probably foreground masks.
The program outputs three pgm images, one for each mask, in the
output folder. These masks are used as input for the grab-cut 
algorithm.
 
### grab-cut

Executes grab-cut algorithm and creates a grab-cut object file.
 

### boundary-correction

Runs the boundary-correction algorithm. Given a grab-cut object file,
the digital curvature flow is evolved in order to enhance segmentation
quality with respect to curvature.

### shape-flow

Evolve the digital curvature flow along digital shapes.


### summary-flow

Overlay flow images in one single image.

## Paper figures
 All figures reproduced in the paper can be generated by
 executing 
 ```
 jmiv-figures/generate-figures.sh [BTOOLS_BINARIES_FOLDER] [EXT_PROJECTS_BINARIES_FOLDER]
 ```


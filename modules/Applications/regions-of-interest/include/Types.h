#ifndef BTools_REGIONSOFINTEREST_TYPES_H
#define BTools_REGIONSOFINTEREST_TYPES_H

namespace RegionsOfInterest
{
    enum ShapeType{Triangle,Square,Pentagon,Heptagon,Ball,Ellipse,Flower,UserDefined,NoType};

    struct Shape
    {
        Shape():type(NoType),imagePath(""),name(""){}

        Shape(ShapeType type, const std::string& imagePath=""):type(type),
                                                                imagePath(imagePath)
        {
            if(type==ShapeType::Triangle) name = "triangle";
            else if(type==ShapeType::Square) name = "square";
            else if(type==ShapeType::Pentagon) name =  "pentagon";
            else if(type==ShapeType::Heptagon) name = "heptagon";
            else if(type==ShapeType::Ball) name = "ball";
            else if(type==ShapeType::Ellipse) name = "ellipse";
            else if(type==ShapeType::Flower) name = "flower";
            else name = "user-defined";
        }

        ShapeType type;
        std::string imagePath;
        std::string name;
    };
}

#endif //BTools_MOSTEXTERN_TYPES_H
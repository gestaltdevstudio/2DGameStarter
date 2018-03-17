#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

namespace GGE
{

    class Model
    {

        public:
            inline virtual ~Model() { };
            virtual void initModel() = 0;
            virtual void step(float deltaTime) = 0;

    };

}

#endif // MODEL_H_INCLUDED

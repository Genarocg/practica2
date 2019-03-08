#ifndef CYLINDER_H
#define CYLINDER_H
#define _USE_MATH_DEFINES
#include <cmath>
#include "Abstractmodel.h"
//recibe parametros; cortes verticales  horizontales, las tapas del cilindro solo son en 2d, rebanadas de pastel (triangulos)
//reter parametyro radio de la atapa superior, 4 parametyro tradio de la tapa inferoir, y la altura del cilindro el quinto
//slices, situado en el centro
//virtual bool: implementa a la clase abstacr model que tiene todo el paso opengl, tenemos que implementarla, omitirla.

class Cylinder : public AbstractModel
{
public:
	Cylinder(int slices, int stacks, float topRadius = 1.0, float bottomRadius = 1.0, float height = 1.0);
	~Cylinder();
	virtual bool rayPicking(glm::vec3 init, glm::vec3 end, glm::vec3 &intersection);
};

#endif // CYLINDER_H
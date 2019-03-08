#ifndef SPHERE_H
#define SPHERE_H
#define _USE_MATH_DEFINES
#include <cmath>
#include "Abstractmodel.h"
//recibe el numero de cortes vetrticales y horizontales ocmo un globo terraqueo
//radio valor por default como en c++
//siempre los parametros por defralut son los ultimos, por que si ponemos otro a trributo al final nos forzamos a ponerle todo completo
class Sphere : public AbstractModel
{
public:
	Sphere(int slices, int stacks, float ratio = 0.5);
	~Sphere();
	virtual bool rayPicking(glm::vec3 init, glm::vec3 end, glm::vec3 &intersection);
private:
	SBB sbb;
};

#endif // SPHERE_H
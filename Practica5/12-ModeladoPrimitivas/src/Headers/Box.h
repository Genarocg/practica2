#ifndef BOX_H
#define BOX_H
#include "Abstractmodel.h"
//no recibe nada, caja de unidades unitarias
class Box : public AbstractModel
{
public:
	Box();
	~Box();
	virtual bool rayPicking(glm::vec3 init, glm::vec3 end, glm::vec3 &intersection);
	AABB aabb;
};

#endif // BOX_H
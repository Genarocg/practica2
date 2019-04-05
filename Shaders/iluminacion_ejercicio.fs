#version 330 core

in vec3 our_color;
in vec3 our_normal;
in vec3 fragPos;
//definimos la estrucura de la luz
struct PositionalLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};


strut material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;


};
out vec4 color;
uniform PositionalLight light;
uniform Material material;
uniform vec3 viewPos;
float shininess;


void main(){
			
	vec3 ambient =light.ambient * our_color;//componente ambiental
	//calcular y nornmalizar el vector normal
	vec3 normal = normalize(our_normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff *light.diffuse * our_color;
	
	//calculando el vector de reflexion r por la funcion reflejo de la - lightDir a la normal

	vec3 r = reflect (-lightDir, normal);
	vec3 viewDir = normalize(viewPos - fragPos);
	float spec = pow(max(dot(r,viewDir),0.0),material.shininess);
	vec3 specular = spec * light.specular *our_color;


	vec3 result = ambient + diff * light.diffuse *  material. diffuse + spec * light.specular * material.specular;


	color = vec4(result, 1.0);

}
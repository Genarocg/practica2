#version 330 core

//este shader le da color  alas cosas, por eso entra un vector de color.

in vec3 our_color;
out vec4 color;
void main(){

color = vec4(our_color, 1.0);

	color = vec4(our_color, 1.0);

}
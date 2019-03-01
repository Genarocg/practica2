//version que trabaja
#version 330 core
//entradas
layout (location =0) in vec3 in_position; // vector de tres elementos para la posicion.la posuision es el 0 del indice
layout (location =1) in vec3 in_color;
//variable d color
out vec3 our_color;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
//metodo main
void main(){


gl_Position = projection * view * model * vec4(in_position, 1.0); //matriz modell vista projection
//le asignamos entero a color
our_color= in_color;
}
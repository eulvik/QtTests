attribute highp vec4 vertex;
attribute lowp vec2 coord;
varying lowp vec2 v_coord;
uniform highp mat4 matrix;

void main() {
   v_coord = coord;
   gl_Position = matrix * vertex;
}

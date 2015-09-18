varying lowp vec2 v_coord;
uniform sampler2D sampler;

void main() {
    gl_FragColor = vec4(texture2D(sampler, v_coord).rgba);
}

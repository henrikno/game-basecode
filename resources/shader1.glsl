@vertex:
void main(void) {
    gl_Position = ftransform(); //gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}
@fragment:
void main(void) {
    gl_FragColor = gl_FragColor;//vec4(1.0,0.0,0.0,0.0);
}

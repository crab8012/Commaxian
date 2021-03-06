uniform sampler2D texture;

void main(){
    // Look up the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // Multiply it by the color
    gl_FragColor = gl_Color * pixel;
}
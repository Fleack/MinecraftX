layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;

out vec2 v_UV;
out vec3 v_Normal;
out vec3 v_FragPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    v_UV        = a_UV;
    v_Normal    = mat3(u_Model) * a_Normal;
    v_FragPos   = vec3(u_Model * vec4(a_Position, 1.0));

    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}

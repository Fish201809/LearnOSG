#version 330 core                                                                 
layout(triangles) in;                                                                  
layout(triangle_strip) out;                                                        
layout(max_vertices = 9) out;                                                        
uniform mat4 osg_ModelViewProjectionMatrix;                                        
in vec4 vcolor[];
in vec3 Norm[]; 
in  vec2 texcoord[];   
out vec2 tex; 
out vec4 fcolor;                                                                   
out vec3 norm;   
uniform sampler2D Tex;
uniform sampler1D Tex1D;
void main(void)                                                                    
{                                                                                    
    for (int i = 0; i < gl_in.length(); i++)                                        
    {                                                                                
        gl_Position = osg_ModelViewProjectionMatrix*gl_in[i].gl_Position;            
       tex=texcoord[i];   
       fcolor=vcolor[i];                                                       
       norm=Norm[i]; 
        EmitVertex();                                                                
    }                                                                                
    EndPrimitive();                                                                    
        gl_Position = osg_ModelViewProjectionMatrix*gl_in[0].gl_Position;            
       tex=texcoord[0];   
       fcolor=vcolor[0];                                                       
       norm=Norm[0]; 
        EmitVertex();                                                                
        gl_Position = osg_ModelViewProjectionMatrix*gl_in[2].gl_Position;            
       tex=texcoord[2];   
       fcolor=vcolor[2];                                                       
       norm=Norm[2]; 
       EmitVertex();                                                                
       gl_Position = osg_ModelViewProjectionMatrix*vec4(1.0,0.0,0.0,1.0);            
       tex=texcoord[1];   
       fcolor=vcolor[2];                                                       
       norm=Norm[0]; 
       EmitVertex();                                                                
    EndPrimitive();                                                                    

} ;
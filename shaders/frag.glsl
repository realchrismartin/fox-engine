  // ==================================================================
  #version 430 core
  
  in vec2 TexCoord; 

  out vec4 FragColor;

  uniform sampler2D textureSampler;

  void main()
  {
    //FragColor = vec4(.5,.5,.5,.5);
    FragColor = texture(textureSampler, TexCoord);
  }

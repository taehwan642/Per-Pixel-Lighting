//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// Default_DirectX_Effect
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string Default_DirectX_Effect_Pass_0_Model : ModelData = ".\\pixellight.x";


float4x4 gWorldMatrix : World;
float4x4 gViewMatrix : View;
float4x4 gProjectionMatrix : ViewProjection;


float4 gWorldLightPosition
<
   string UIName = "gWorldLightPosition";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 500.00, 500.00, -500.00, 1.00 );
float4 gWorldCameraPosition : ViewPosition;


struct VS_INPUT
{
   float4 mPosition : POSITION;
   float3 mNormal: NORMAL;
};


struct VS_OUTPUT
{
   float4 mPosition : POSITION;
   float3 mLightDir : TEXCOORD1;
   float3 mViewDir: TEXCOORD2;
   float3 mWorldNormal : TEXCOORD3;
};


VS_OUTPUT Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;


   Output.mPosition = mul( Input.mPosition, gWorldMatrix );


   float3 lightDir = Output.mPosition.xyz - gWorldLightPosition.xyz;
   Output.mLightDir = (lightDir);

   float3 viewDir = (Output.mPosition.xyz - gWorldCameraPosition.xyz);
   Output.mViewDir = viewDir;

   Output.mPosition = mul( Output.mPosition, gViewMatrix );
   Output.mPosition = mul( Output.mPosition, gProjectionMatrix );

   float3 worldNormal = mul( Input.mNormal, (float3x3)gWorldMatrix );
   Output.mWorldNormal = (worldNormal);


   return Output;
}





struct PS_INPUT
{
   float4 mPosition : POSITION;
   float3 mLightDir : TEXCOORD1;
   float3 mViewDir: TEXCOORD2;
   float3 mWorldNormal : TEXCOORD3;
};

float4 Default_DirectX_Effect_Pass_0_Pixel_Shader_gWorldLightPosition;

float4 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
   float3 specular = 0;// = float3(0.3f, 0.3f, 0.3f);
   float3 LightColor = float3(1,1,1);
   float LightPower = 50.0f;
   float dist = length(Input.mLightDir - Input.mPosition);
   float3 n = normalize(Input.mWorldNormal);
   float3 l = normalize(Input.mLightDir);
   float cosTheta = clamp( dot( n,-l ), 0,1 );
   float3 E = normalize(Input.mViewDir);
   float3 R = reflect(-l,n);
   float cosAlpha = clamp( dot( E,R ), 0,1 );

   //float3 re = reflect(Input.mLightDir, Input.mWorldNormal);
   //float3 nreflect = normalize(re);
   //float3 viewDir = normalize(Input.mViewDir);
   //float3 specular = 0;
   
   if ( cosTheta.x > 0 )
   {
      specular = saturate(dot(R, -E ));
      specular = pow(specular, 20.0f);
   }
   
   float3 ambient = float3(0.1f, 0.1f, 0.1f);
   float cosAlphare = pow(cosAlpha, 5) / (dist * dist);
   float3 result = 
   ambient + 
   specular + 
   cosTheta;// * LightColor * LightPower;// * cosAlphare;
   //result.x = result.x * 1.0f;
   
   return float4(result,1);
}

//--------------------------------------------------------------//
// Technique Section for Default_DirectX_Effect
//--------------------------------------------------------------//
technique Default_DirectX_Effect
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main();
   }

}


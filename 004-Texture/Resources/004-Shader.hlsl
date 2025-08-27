struct VSInput
{
    float4 Position : POSITION;
    float2 TexCoordUV : TEXCOORD;
};

struct VSOutput
{
    float4 Position : SV_POSITION;
    float2 TexCoordUV : TEXCOORD;
};

VSOutput VSMain(VSInput Input)
{
    VSOutput Output;
    Output.Position = Input.Position;
    Output.TexCoordUV = Input.TexCoordUV;

    return Output;
}

Texture2D mTexture : register(T0, Space0);
SamplerState mSampler : register(S0, Space0);

float4 PSMain(VSOutput Input) : SV_Target
{
    return mTexture.Sample(mSampler, Input.TexCoordUV);
}
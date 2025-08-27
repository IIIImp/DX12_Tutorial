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

cbuffer GlobalData : register(b0, space0)
{
    row_major float4x4 MVP;
}

VSOutput VSMain(VSInput Input)
{
    VSOutput Output;
    Output.Position = mul(Input.Position, MVP);
    Output.TexCoordUV = Input.TexCoordUV;

    return Output;
}

Texture2D mTexture : register(t0, space0);
SamplerState mSampler : register(s0, space0);

float4 PSMain(VSOutput Input) : SV_Target
{
    return mTexture.Sample(mSampler, Input.TexCoordUV);
}
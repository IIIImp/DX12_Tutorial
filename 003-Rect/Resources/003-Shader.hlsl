struct VSInput
{
    float4 Position : POSITION;
    float4 Color : COLOR;
};

struct VSOutput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

VSOutput VSMain(VSInput Input)
{
    VSOutput Output;
    Output.Position = Input.Position;
    Output.Color = Input.Color;

    return Output;
}

float4 PSMain(VSOutput Input) : SV_Target
{
    return Input.Color;
}
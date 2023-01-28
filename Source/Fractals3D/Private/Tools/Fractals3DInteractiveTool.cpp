// Copyright Epic Games, Inc. All Rights Reserved.

#include "Fractals3DInteractiveTool.h"
#include "InteractiveToolManager.h"
#include "Interfaces/IPluginManager.h"
#include "ToolBuilderUtil.h"
#include <iostream>
#include <fstream>
#include "BaseBehaviors/ClickDragBehavior.h"
#include "SourceControlHelpers.h"

#include "SceneManagement.h"

// localization namespace
#define LOCTEXT_NAMESPACE "UFractals3DInteractiveTool"

/*
 * ToolBuilder
 */

UInteractiveTool* UFractals3DInteractiveToolBuilder::BuildTool(const FToolBuilderState & SceneState) const
{
	UFractals3DInteractiveTool* NewTool = NewObject<UFractals3DInteractiveTool>(SceneState.ToolManager);
	return NewTool;
}


// JAH TODO: update comments
/*
 * Tool
 */

UFractals3DInteractiveToolProperties::UFractals3DInteractiveToolProperties()
{
	// initialize the points and distance to reasonable values
	FractalName = "Default";
	LastSDF = FractalConfigSDF::Mandelbrot;
}


void UFractals3DInteractiveTool::Setup()
{
	UInteractiveTool::Setup();

	// Create the property set and register it with the Tool
	Properties = NewObject<UFractals3DInteractiveToolProperties>(this, "Measurement");
	AddToolPropertySource(Properties);
}


void UFractals3DInteractiveTool::OnUpdateModifierState(int ModifierID, bool bIsOn)
{
	
}


FInputRayHit UFractals3DInteractiveTool::CanBeginClickDragSequence(const FInputDeviceRay& PressPos)
{
	return FInputRayHit();
}


void UFractals3DInteractiveTool::OnClickPress(const FInputDeviceRay& PressPos)
{
	//UpdatePosition(PressPos.WorldRay);
}


void UFractals3DInteractiveTool::OnClickDrag(const FInputDeviceRay& DragPos)
{
	//UpdatePosition(DragPos.WorldRay);
}

void UFractals3DInteractiveTool::OnPropertyModified(UObject* PropertySet, FProperty* Property)
{
	// if the user updated any of the property fields, update the distance
	//UpdateDistance();
	GenerateFractal();
}

FString FunctionNameBySDFType(FractalConfigSDF Type)
{
	switch (Type)
	{
		case FractalConfigSDF::PlaneFold: return "planeFold";
		case FractalConfigSDF::AbsFold: return "absFold";
		case FractalConfigSDF::SierpinskiFold: return "sierpinskiFold";
		case FractalConfigSDF::MengerFold: return "mengerFold";
		case FractalConfigSDF::SphereFold: return "sphereFold";
		case FractalConfigSDF::BoxFold: return "boxFold";
		case FractalConfigSDF::RotXFold: return "rotXFold";
		case FractalConfigSDF::RotYFold: return "rotYFold";
		case FractalConfigSDF::RotZFold: return "rotZFold";
		case FractalConfigSDF::ScaleTranslateFold: return "scaleTranslateFold";
		case FractalConfigSDF::ScaleOriginFold: return "scaleOriginFold";
		case FractalConfigSDF::OrbitColoring: return "orbitByType";
		case FractalConfigSDF::Cone: return "sdCone";
		case FractalConfigSDF::HexPrism: return "sdHexPrism";
		case FractalConfigSDF::Sphere: return "sdSphere";
		case FractalConfigSDF::Capsule: return "sdCapsule";
		case FractalConfigSDF::Torus: return "sdTorus";
		case FractalConfigSDF::Box: return "sdBox";
		case FractalConfigSDF::Tetrahedron: return "sdTetrahedron";
		case FractalConfigSDF::InfCross: return "sdInfCross";
		case FractalConfigSDF::InfCrossXY: return "sdInfCrossXY";
		case FractalConfigSDF::InfLine: return "sdInfLine";
		case FractalConfigSDF::Mandelbox: return "sdMandelBox";
		case FractalConfigSDF::ButterweedHiils: return "sdButterweedHills";
		case FractalConfigSDF::Mausoleum: return "sdMausoleum";
		case FractalConfigSDF::SnowStadium: return "sdSnowStadium";
		case FractalConfigSDF::Menger: return "sdMenger";
		case FractalConfigSDF::Sierpinski: return "sdSerpinski";
		case FractalConfigSDF::Tree: return "sdTree";
		case FractalConfigSDF::Julia2: return "sdJulia2";
		case FractalConfigSDF::Julia: return "sdJulia";
		case FractalConfigSDF::Mandelbrot: return "sdMondelbrot";
	}

	return "Error";
}

FString FunctionArgumentsBySDFType(FractalConfigSDF Type)
{
	switch (Type)
	{
		case FractalConfigSDF::PlaneFold: return "(new_p, float3(0, 0, -1), -1)";
		case FractalConfigSDF::AbsFold: return "(new_p, float3(0.0f, 0.0f, 0.0f))";
		case FractalConfigSDF::SierpinskiFold: return "(new_p)";
		case FractalConfigSDF::MengerFold: return "(new_p)";
		case FractalConfigSDF::SphereFold: return "(new_p, 0.5, 1.0f)";
		case FractalConfigSDF::BoxFold: return "(new_p, FoldSize)";
		case FractalConfigSDF::RotXFold: return "(new_p, FoldAngles.x)";
		case FractalConfigSDF::RotYFold: return "(new_p, FoldAngles.y)";
		case FractalConfigSDF::RotZFold: return "(new_p, FoldAngles.z)";
		case FractalConfigSDF::ScaleTranslateFold: return "(new_p, FoldingScale, Power * FoldOffset)";
		case FractalConfigSDF::ScaleOriginFold: return "(new_p, FoldingScale)";
		case FractalConfigSDF::OrbitColoring: return "(orbit, new_p)";
		case FractalConfigSDF::Cone: return "(new_p, float2(0.0f, 0.0f), Offset)";
		case FractalConfigSDF::HexPrism: return "(new_p, float2(Offset, Offset))";
		case FractalConfigSDF::Sphere: return "(new_p, Offset)";
		case FractalConfigSDF::Capsule: return "(new_p, FoldingScale, Offset)";
		case FractalConfigSDF::Torus: return "(new_p, float2(Offset, Offset))";
		case FractalConfigSDF::Box: return "(new_p, float3(Offset, Offset, Offset))";
		case FractalConfigSDF::Tetrahedron: return "(new_p, Offset)";
		case FractalConfigSDF::InfCross: return "(new_p, Offset)";
		case FractalConfigSDF::InfCrossXY: return "(new_p, Offset)";
		case FractalConfigSDF::InfLine: return "(new_p, Offset)";
		case FractalConfigSDF::Mandelbox: return "(new_p).x";
		case FractalConfigSDF::ButterweedHiils: return "(new_p).x";
		case FractalConfigSDF::Mausoleum: return "(new_p).x";
		case FractalConfigSDF::SnowStadium: return "(new_p).x";
		case FractalConfigSDF::Menger: return "(new_p).x";
		case FractalConfigSDF::Sierpinski: return "(new_p).x";
		case FractalConfigSDF::Tree: return "(new_p).x";
		case FractalConfigSDF::Julia2: return "(new_p).x";
		case FractalConfigSDF::Julia: return "(new_p).x";
		case FractalConfigSDF::Mandelbrot: return "(new_p).x";
	}
	return "Error";
}

bool IsOrbit(FractalConfigSDF Type)
{
	return Type == FractalConfigSDF::OrbitColoring;
}

void UFractals3DInteractiveTool::GenerateFractal() {
	FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("Fractals3D"))->GetBaseDir(), TEXT("Shaders"));
	FString GeneratedShadersDir = FPaths::Combine(PluginShaderDir, TEXT("GeneratedSDF"));
	FString CurrentShaderDir = FPaths::Combine(GeneratedShadersDir, Properties->FractalName);
	if (FPaths::DirectoryExists(CurrentShaderDir))
	{
		// Remove old shaders, keep material
	}

	// Fill MainShader
	FString MainShaderFilename = Properties->FractalName;
	MainShaderFilename += ".ush";
	FString MainShader = "#include \"/PluginShaders/SDFractalLibraryNew.ush\"\n"
		"\n"
		"SDFractal sdfLibrary;\n"
		"\n"
		"struct SDF {\n"
		"#include \"/PluginShaders/GeneratedSDF/";
	MainShader += Properties->FractalName;
	MainShader += "SDF.ush\"\n"
		"};\n"
		"\n"
		"#include \"/PluginShaders/RayMarchingFractalNew.ush\"\n"
		;

	// Fill SDF Shader
	FString SdfShaderFilename = Properties->FractalName;
	SdfShaderFilename += "SDF.ush";
	FString SdfShader = "float2 sdf(float3 p, out float3 outputColor) {\n"
		"	outputColor = orbitInitInf();\n"
		"	float4 new_p = float4(p, 1.0f);\n"
		"	float d = 1e20;\n"
		"	for (int i = 0; i < Iterations; i++) {\n"
		"";

	for (int i = 0; i < Properties->FractalConfig.Num(); ++i)
	{
		if (IsOrbit(Properties->FractalConfig[i]))
			SdfShader += "		orbit = orbitByType(orbit, new_p);\n";
		else
		{
			SdfShader += "		new_p = ";
			SdfShader += FunctionNameBySDFType(Properties->FractalConfig[i]);
			SdfShader += FunctionArgumentsBySDFType(Properties->FractalConfig[i]);
			SdfShader += ";\n";
		}
	}

	SdfShader += "\n"
		"	}\n";

	SdfShader += "\n"
		"	return float2(min(d, ";
	SdfShader += FunctionNameBySDFType(Properties->LastSDF);
	SdfShader += FunctionArgumentsBySDFType(Properties->LastSDF);
	SdfShader += "), length(new_p));\n"
		"}\n";

	std::ofstream foutMainShader(*FPaths::Combine(CurrentShaderDir, MainShaderFilename));
	foutMainShader << *MainShader;

	std::ofstream foutSdfShader(*FPaths::Combine(CurrentShaderDir, SdfShaderFilename));
	foutSdfShader << *SdfShader;
}


#undef LOCTEXT_NAMESPACE
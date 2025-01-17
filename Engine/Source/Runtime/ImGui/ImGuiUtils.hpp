#pragma once 

#include "ECWorld/SceneWorld.h"
#include "ECWorld/TransformComponent.h"
#include "ImGui/ImGuiBaseLayer.h"
#include "ImGui/ImGuiContextInstance.h"
#include "Math/UnitSystem.hpp"

#include <imgui/imgui.h>

namespace ImGuiUtils
{

static bool ImGuiBoolProperty(const char* pName, bool& value)
{
	return ImGui::Checkbox(pName, &value);
}

template<typename EnumType>
static bool ImGuiEnumProperty(const char* pName, EnumType& value)
{
	bool dirty = false;

	ImGui::Columns(2);
	ImGui::TextUnformatted(pName);
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	if (ImGui::BeginCombo("##combo", nameof::nameof_enum(value).data()))
	{
		auto enumCount = nameof::enum_count<EnumType>();
		for (uint32_t enumIndex = 0U; enumIndex < enumCount; ++enumIndex)
		{
			EnumType enumValue = static_cast<EnumType>(enumIndex);
			bool isSelected = enumValue == value;
			if (ImGui::Selectable(nameof::nameof_enum(enumValue).data(), isSelected))
			{
				value = enumValue;
				dirty = true;
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();
	ImGui::Columns(1);

	return dirty;
}

static bool ImGuiStringProperty(const char* pName, const char* pValue)
{
	ImGui::Columns(2);
	ImGui::TextUnformatted(pName);
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	ImGui::TextUnformatted(pValue);

	ImGui::PopItemWidth();
	ImGui::NextColumn();
	ImGui::Columns(1);

	// TODO : editable
	return false;
}

static bool ImGuiStringProperty(const char* pName, const std::string& value)
{
	return ImGuiStringProperty(pName, value.c_str());
}

static bool ImGuiIntProperty(const char* pName, int& value, cd::Unit unit = cd::Unit::None, int minValue = {}, int maxValue = {}, bool isNormalized = false, float speed = -1.0f)
{
	bool dirty = false;

	ImGui::Columns(2);
	ImGui::TextUnformatted(pName);
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	if (ImGui::DragInt(pName, &value, speed, minValue, maxValue, "%d"))
	{
		dirty = true;
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();
	ImGui::Columns(1);

	return dirty;
}

static bool ImGuiFloatProperty(const char* pName, float& value, cd::Unit unit = cd::Unit::None, float minValue = {}, float maxValue = {}, bool isNormalized = false, float speed = -1.0f)
{
	bool dirty = false;

	ImGui::Columns(2);
	ImGui::TextUnformatted(pName);
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	//std::string metricName = std::format("%.2f{}", cd::GetUnitName(unit));
	std::string metricName = "%.2f";
	metricName += cd::GetUnitName(unit);
	float delta = maxValue - minValue;
	float dragSpeed = (speed <= 0.0) ? (cd::Math::IsEqualToZero(delta) ? 1.0f : delta * 0.05f) : speed;
	if (ImGui::DragFloat(pName, &value, dragSpeed, minValue, maxValue, metricName.c_str()))
	{
		dirty = true;
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();
	ImGui::Columns(1);

	return dirty;
}

template<typename T>
static bool ImGuiVectorProperty(const char* pName, T& value, cd::Unit unit = cd::Unit::None, const T& minValue = {}, const T& maxValue = {}, bool isNormalized = false, float speed = -1.0f)
{
	bool dirty = false;

	if (isNormalized)
	{
		value.Normalize();
	}

	ImGui::Columns(2);
	ImGui::TextUnformatted(pName);
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	//std::string metricName = std::format("%.2f{}", cd::GetUnitName(unit));
	std::string metricName = "%.2f";
	metricName += cd::GetUnitName(unit);
	float delta = maxValue.x() - minValue.x();
	float dragSpeed = (speed <= 0.0) ? (cd::Math::IsEqualToZero(delta) ? 1.0f : delta * 0.05f) : speed;
	if constexpr (std::is_same<T, cd::Vec2f>())
	{
		if (ImGui::DragFloat2(pName, value.Begin(), dragSpeed, minValue.x(), maxValue.x(), metricName.c_str()))
		{
			dirty = true;
		}
	}
	else if constexpr (std::is_same<T, cd::Vec3f>())
	{
		if (ImGui::DragFloat3(pName, value.Begin(), dragSpeed, minValue.x(), maxValue.x(), metricName.c_str()))
		{
			dirty = true;
		}
	}
	else if constexpr (std::is_same<T, cd::Vec4f>())
	{
		if (ImGui::DragFloat4(pName, value.Begin(), dragSpeed, minValue.x(), maxValue.x(), metricName.c_str()))
		{
			dirty = true;
		}
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();
	ImGui::Columns(1);

	return dirty;
}

static bool ImGuiTransformProperty(const char* pName, cd::Transform& value)
{
	bool dirty = false;
	if (ImGuiVectorProperty("Translation", value.GetTranslation()))
	{
		dirty = true;
	}

	cd::Vec3f eularAngles = value.GetRotation().ToEulerAngles();
	if (ImGuiVectorProperty("Rotation", eularAngles, cd::Unit::Degree, cd::Vec3f::Zero(), cd::Vec3f(360.0f)))
	{
		float pitch = std::min(eularAngles.x(), 89.9f);
		pitch = std::max(pitch, -89.9f);

		value.SetRotation(cd::Quaternion::FromPitchYawRoll(pitch, eularAngles.y(), eularAngles.z()));
		dirty = true;
	}

	cd::Vec3f originScale = value.GetScale();
	cd::Vec3f scale = originScale;
	ImGui::TextUnformatted("Scale");
	ImGui::SameLine();
	bool UniformScaleEnabled = engine::TransformComponent::DoUseUniformScale();
	ImGui::Checkbox("Uniform", &UniformScaleEnabled);
	engine::TransformComponent::SetUseUniformScale(UniformScaleEnabled);

	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	if (ImGui::DragFloat3("##Scale", scale.Begin(), 0.1f, 0.001f, 999.0f))
	{
		if (!cd::Math::IsEqualTo(scale.x(), originScale.x()))
		{
			if (UniformScaleEnabled)
			{
				float ratio = scale.x() / originScale.x();
				cd::Vec3f _scale = value.GetScale();
				_scale *= ratio;
				value.SetScale(_scale);

				dirty = true;
			}
			else
			{
				value.SetScale(scale);
				dirty = true;
			}
		}

		if (!cd::Math::IsEqualTo(scale.y(), originScale.y()))
		{
			if (UniformScaleEnabled)
			{
				float ratio = scale.y() / originScale.y();
				cd::Vec3f _scale = value.GetScale();
				_scale *= ratio;
				value.SetScale(_scale);
				dirty = true;
			}
			else
			{
				value.SetScale(scale);
				dirty = true;
			}
		}

		if (!cd::Math::IsEqualTo(scale.z(), originScale.z()))
		{
			if (UniformScaleEnabled)
			{
				float ratio = scale.z() / originScale.z();
				cd::Vec3f _scale = value.GetScale();
				_scale *= ratio;
				value.SetScale(_scale);
				dirty = true;
			}
			else
			{
				value.SetScale(scale);
				dirty = true;
			}
		}
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();
	ImGui::Columns(1);

	return dirty;
}

static void ColorPickerProperty(const char* Name, cd::Vec3f& veccolor)
{
	static std::map<const char*, bool> showMap;
	if (!showMap.count(Name))
	{
		showMap[Name] = false;
	}
	ImGui::TextUnformatted(Name);
	ImGui::SameLine();
	ImGui::NextColumn();
	ImGui::PushID(Name);
	if (ImGui::Button("..."))
	{
		showMap[Name] = true;
	}
	ImGui::PopID();
	ImGui::PushItemWidth(-1);
	ImGui::SameLine();
	ImGui::NextColumn();
	ImGui::DragFloat3("", veccolor.Begin(), 0, 0.0f, 1.0f);
	ImGui::PopItemWidth();
	if (showMap[Name])
	{
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 mainWindowSize = io.DisplaySize;
		float offsetX = 400;
		float offsetY = 400;
		ImVec2 windowPos(mainWindowSize.x - offsetX, mainWindowSize.y - offsetY);

		ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
		ImGui::Begin(Name, &showMap[Name], ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::ColorPicker3("Color Picker", veccolor.Begin());
		ImGui::End();
	}
	ImGui::Separator();
}

}

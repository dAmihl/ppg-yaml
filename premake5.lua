workspace "PPG-YAML"
	language "C++"
	cppdialect "C++14"
	configurations { "Debug", "Release"}
	
	include "vendor/ppg-core/ppg-core.lua"
	
	include "ppg-yaml.lua"
			
--------------- Example
project ("PPG-YAML-example")
	location "example"
	targetdir "bin"
	kind "ConsoleApp"
	files {"example/*.cpp"}
	includedirs {"src", "vendor/ppg-core/src", "vendor/yaml-cpp/include"}
	links {"PPG-YAML", "PPG", "yaml-cpp"}
	
	filter "configurations:Debug"
		defines {"DEBUG"}
		symbols "On"
		
	filter "configurations:Release"
		defines {"NDEBUG"}
		symbols "On"
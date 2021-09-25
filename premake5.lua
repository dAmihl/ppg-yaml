workspace "PPG-YAML"
	language "C++"
	cppdialect "C++14"
	configurations { "Debug", "Release"}
	
	include "vendor/ppg-core/ppg-core.lua"
	
project "yaml-cpp"
	kind "StaticLib"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	
	files{"vendor/yaml-cpp/src/**.h", "vendor/yaml-cpp/src/**.cpp"}
	
	includedirs {"vendor/yaml-cpp/include"}
	
	filter "configurations:Debug"
		defines {"DEBUG"}
		symbols "On"
		
	filter "configurations:Release"
		defines {"NDEBUG"}
		symbols "On"
	
project "PPG-YAML"
	kind "StaticLib"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	
	links {"PPG", "yaml-cpp"}
	
	files {"src/**.h", "src/**.cpp"}
	
	includedirs {"vendor/ppg-core/src", "vendor/yaml-cpp/include"}
	
	filter "configurations:Debug"
		defines {"DEBUG"}
		symbols "On"
		
	filter "configurations:Release"
		defines {"NDEBUG"}
		symbols "On"
			
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
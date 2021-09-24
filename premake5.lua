workspace "PPG-YAML"
	language "C++"
	cppdialect "C++14"
	configurations { "Debug", "Release"}
	
project "PPG-YAML"
	kind "StaticLib"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	
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
	includedirs {"src", "vendor/ppg-core/src"}
	links {"PPG-YAML"}
	
	filter "configurations:Debug"
		defines {"DEBUG"}
		symbols "On"
		
	filter "configurations:Release"
		defines {"NDEBUG"}
		symbols "On"
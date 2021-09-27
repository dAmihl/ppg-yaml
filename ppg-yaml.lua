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
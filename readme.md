### Server: 
https://github.com/cjsjyh/krafton-internship-server

### Client: 
https://github.com/cjsjyh/krafton-internship-client



DirectX 설치:
https://www.microsoft.com/en-us/download/confirmation.aspx?id=6812

필요한 Visual Studio Library:
https://devblogs.microsoft.com/cppblog/directx-game-development-with-c-in-visual-studio/

boost 다운로드:
https://www.boost.org/

boost build:
run boostrap.bat

boost compile:
b2 release debug threading=multi --build-type=complete --toolset=msvc address-model=64 stage

Visual Studio Directory:
Linker > General > Additional Library Directory : <boost path>\stage\lib

VC++ Directory > Include Directory : <directX path>\include; <boost path>
C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include;
C:\Users\Junsoo\Downloads\boost_1_70_0

VC++ Directory > Library Directory: <directX path>\Lib\(x86 or x64)



Similar Project:
https://www.youtube.com/watch?v=uk11zW8ha-Q

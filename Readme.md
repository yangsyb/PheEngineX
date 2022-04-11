# 启动项目

# 操作按键
按住鼠标左键滑动可调整视角，按住鼠标左键的同时按W A S D可进行方向移动, 如果相机移动过快或过慢可调整，按Z加速，按X减速

# 场景
默认多线程渲染 <br/>
按1可渲染一个box <br/>
按2渲染导出的json地图 + PBR炮台 <br/>
按3渲染地图 + WPO树 + PBR炮台 <br/>
按5可开关灯是否移动 <br/>


# Engine Design
Engine/Core : Basic Infrastructure Used for Engine Construction <br/><br/>
Engine/Editor : Like ContentBrowser in UE5 <br/>
These are Used for USER to manipulate the Engine <br/>
e.g. <br/>
Create A Texture On your ContentBrowser <br/>
Create An Actor On your ContentBrowser <br/>
(May not have a Viewable Editor so far but structures are designed) <br/><br/>
Engine/Scene : MainThread Scene Manager, all NODE are used for Construct MainThreadScene <br/>

Render : RenderThread staff (Renderer, RenderScene, Primitive) <br/>

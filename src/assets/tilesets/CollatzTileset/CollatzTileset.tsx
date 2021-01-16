<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.4" tiledversion="1.4.3" name="CollatzTileset2" tilewidth="271" tileheight="273" tilecount="6" columns="6">
 <properties>
  <property name="rotation" type="float" value="45"/>
 </properties>
 <image source="CollatzTileset.png" width="1626" height="273"/>
 <tile id="0" type="COLLATZ_TILE_0"/>
 <tile id="1" type="COLLATZ_TILE_1"/>
 <tile id="2" type="COLLATZ_TILE_2"/>
 <tile id="3" type="COLLATZ_TILE_3"/>
 <tile id="4" type="COLLATZ_TILE_4"/>
 <tile id="5" type="COLLATZ_TILE_5"/>
 <wangsets>
  <wangset name="WangCollatz" tile="-1">
   <wangedgecolor name="0,3" color="#ff0000" tile="-1" probability="1"/>
   <wangedgecolor name="0,2" color="#00ff00" tile="-1" probability="1"/>
   <wangedgecolor name="1,2" color="#0000ff" tile="-1" probability="1"/>
   <wangedgecolor name="1,3" color="#ff7700" tile="-1" probability="1"/>
   <wangedgecolor name="2,3" color="#00e9ff" tile="-1" probability="1"/>
   <wangtile tileid="0" wangid="0x2010201"/>
   <wangtile tileid="1" wangid="0x2010304"/>
   <wangtile tileid="2" wangid="0x2040205"/>
   <wangtile tileid="3" wangid="0x3040301"/>
   <wangtile tileid="4" wangid="0x3050204"/>
   <wangtile tileid="5" wangid="0x3050305"/>
  </wangset>
 </wangsets>
</tileset>

<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.4" tiledversion="1.4.3" name="CollatzTileset" tilewidth="347" tileheight="347" tilecount="6" columns="0">
 <grid orientation="orthogonal" width="1" height="1"/>
 <properties>
  <property name="rotation" type="float" value="0.25"/>
 </properties>
 <tile id="0" type="COLLATZ_TILE_0">
  <image width="271" height="273" source="CollatzTile0.png"/>
 </tile>
 <tile id="1" type="COLLATZ_TILE_1">
  <image width="270" height="272" source="CollatzTile1.png"/>
 </tile>
 <tile id="2" type="COLLATZ_TILE_2">
  <image width="271" height="272" source="CollatzTile2.png"/>
 </tile>
 <tile id="3" type="COLLATZ_TILE_3">
  <image width="269" height="272" source="CollatzTile3.png"/>
 </tile>
 <tile id="4" type="COLLATZ_TILE_4">
  <image width="270" height="271" source="CollatzTile4.png"/>
 </tile>
 <tile id="5" type="COLLATZ_TILE_5">
  <image width="269" height="272" source="CollatzTile5.png"/>
 </tile>
 <wangsets>
  <wangset name="WangCollatz" tile="-1">
   <wangedgecolor name="" color="#ff0000" tile="-1" probability="1"/>
   <wangedgecolor name="" color="#00ff00" tile="-1" probability="1"/>
   <wangedgecolor name="" color="#0000ff" tile="-1" probability="1"/>
   <wangedgecolor name="" color="#ff7700" tile="-1" probability="1"/>
   <wangedgecolor name="" color="#00e9ff" tile="-1" probability="1"/>
   <wangtile tileid="0" wangid="0x2010201"/>
   <wangtile tileid="1" wangid="0x2010304"/>
   <wangtile tileid="2" wangid="0x2040205"/>
   <wangtile tileid="3" wangid="0x3040301"/>
   <wangtile tileid="4" wangid="0x3050204"/>
   <wangtile tileid="5" wangid="0x3050305"/>
  </wangset>
 </wangsets>
</tileset>

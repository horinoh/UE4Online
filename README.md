# UE4Online

## 使用アセット

* Animation Starter Pack
    * https://www.unrealengine.com/marketplace/animation-starter-pack

# インプット

* Action Mapping
    * Jump, Crouch を追加
    
* Axis Mapping
    * MoveForward, MoveRight, Turn, Lookup を追加

##

Project Settings - Map & Modes - Default GameMode, GameInstance Class を指定

<!--
Project Settings - General Settings - Game Viewport Client Class, を指定
-->

## [Steam](https://github.com/horinoh/UE4Steam/tree/master/)

## 起動
* 2PC で .uproject 右クリック - Launch Game で立ち上げる (1PCでやる場合は OnlineSubsystemNull にして２窓起動する)
  * 一方を Create Session - OK
  * もう一方を Find Session - OK
    * セッションが見つかったらその行をダブルクリックする、入れたらOK
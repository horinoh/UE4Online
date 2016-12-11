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

## Steam

* DefaultEngine.ini の設定を貼り付ける
  * https://docs.unrealengine.com/latest/INT/Programming/Online/Steam/index.html#iniconfiguration
* XXX.Build.cs のコメントアウトされている OnlineSubsystem を有効にする
  * 必要なら OnlineSubSystemUtils も追加
* XXX.Target.cs のコンストラクタに bUsesSteam = true を追記
* XXX.uproject に OnlineSubsytemSteam プラグインの項目を追加
* スタンドアロンで起動する(PIEではダメ)
  * XXX.uproject 右クリック - Launch game または エディタから Standalone Game で起動する
  * 右下に Steam のオーバーレイが表示されたら OK
  
* オンライン対応には 2PC、2Steam アカウントが必要

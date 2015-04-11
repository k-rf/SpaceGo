#ifndef _ENTITY_H
#define _ENTITY_H

#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "input.h"
#include "game.h"

namespace entityNS
{
	enum COLLISION_TYPE { NONE, CIRCLE, BOX, ROTATED_BOX, PIXEL_PERFECT };
	const float GRAVITY = 6.67428e-11f;
}

class Entity : public Image
{
protected:
	entityNS::COLLISION_TYPE collisionType;      // 衝突の種類
	VECTOR2 center;          // エンティティの中心
	float radius;            // 円の衝突の判定に使用する、円の半径
	VECTOR2 distSquared;
	float sumRadiiSquared;
	RECT edge;               // BOXまたはROTATED_BOXの衝突判定が使用する矩形領域
	VECTOR2 corners[4];      // ROTATED_BOXの衝突判定が使用する矩形領域の頂点
	VECTOR2 edge01, edge03;  // 投影に用いられる
	float edge01Min, edge01Max, edge03Min, edge03Max;
	VECTOR2 velocity;        // 速度
	VECTOR2 deltaV;
	float mass;              // エンティティの質量
	float health;            // エンティティの体力
	float rr;
	float force;
	float gravity;
	Input* input;
	HRESULT hr;
	bool active;
	bool rotatedBoxReady;
	DWORD pixelsColliding;

	// 円の衝突判定メソッド
	// collision()によって呼び出される、デフォルトの衝突判定メソッド
	// 実行後：衝突している場合はtrue、していない場合はfalseを戻す
	// 衝突している場合は、collisionVectorを設定
	virtual bool collideCircle(Entity& ent, VECTOR2& collisionVector);

	// 軸平行境界ボックスの衝突判定メソッド
	// collision()によって呼び出される
	// 実行後：衝突している場合はtrue、していない場合はfalseを戻す
	// 衝突している場合は、collisionVectorを設定
	virtual bool collideBox(Entity& ent, VECTOR2& collisionVector);

	// 回転するボックスの衝突判定メソッド
	// collision()によって呼び出される
	// 実行後：衝突している場合はtrue、していない場合はfalse
	// 　　　　衝突している場合は、collisionVectorを設定
	// 分離軸テストを使用して衝突を判定
	// 分離軸テスト：
	// ２つのボックスは、特定の直線への投影が重なっていなければ衝突していない
	virtual bool collideRotatedBox(Entity& ent, VECTOR2& collisionVector);

	// 回転するボックスと円の衝突判定メソッド
	// collision()によって呼び出される
	// ボックスの辺と円の半径で分離軸テストを使用
	// 円の中心が衝突ボックスの各辺を伸ばした直線の外側
	// （ボロノイ領域と呼ばれる）にある場合、ボックスのもっとも近い頂点と
	// 衝突がないかを距離のチェックで判定します。
	// 最も近い頂点は、重なりのテストから判断できます。
	//
	// Voronoi0 |   | Voronoi1
	//       ---0---1---
	//          |   |
	//       ---3---2---
	// Voronoi3 |   | Vronoi2
	//
	// 実行前：このエンティティは必ずボックス、相手のエンティティ（ent）は必ず円
	// 実行後：衝突している場合はtrue、していない場合はfalseを戻す
	// 衝突している場合は、collisionVectorを設定
	virtual bool collideRotatedBoxCircle(Entity& ent, VECTOR2& collisionVector);

	// 回転するボックスの頂点、投影線、投影の最小値と最大値を計算
	// 0---1 頂点の番号
	// |   |
	// 3---2
	void computeRotatedBox();

	// 相手のボックスを、このエンティティのedge01およびedge03に投影
	// collideRotatedVox()によって呼び出される
	// 実行後：投影が重なっている場合はtrue、それ以外の場合はfalseを戻す
	bool projectionsOverlap(Entity& ent);

	// ボックスの頂点が円と衝突していないかを、距離のチェックを使用して判定
	// collideRotatedBoxCircle()によって呼び出される
	// 実行後：衝突している場合はtrue、していない場合はfalseを戻す
	// 衝突している場合は、collisionVectorを設定
	bool collideCornerCircle(VECTOR2 corner, Entity& ent, VECTOR2& collisionVector);

public:
	// コンストラクタ
	Entity();

	// デストラクタ
	virtual ~Entity(){}

	// ------------------------------------------------------------------------
	// ゲッタ
	// ------------------------------------------------------------------------

	// エンティティの中心を返す
	virtual const VECTOR2* getCenter()
	{
		center = VECTOR2(getCenterX(), getCenterY());
		return &center;
	}

	// 衝突判定の円の半径を返す
	virtual float getRadius()const { return radius; }

	// BOXとROTATED_BOXの衝突判定に使われているRECT構造体を返す
	virtual const RECT& getEdge()const { return edge; }

	// ROTATED_BOXの頂点cを返す
	virtual const VECTOR2* getCorner(UINT c)const
	{
		if(c >= 4)
			c = 0;
		return &corners[c];
	}

	// 速度ベクトルを返す
	virtual const VECTOR2 getVelocity()const { return velocity; }

	// activeを返す
	virtual bool getActive()const { return active; }

	// 質量を返す
	virtual float getMass()const { return mass; }

	// 重力定数を返す
	virtual float getGravity()const { return gravity; }

	// 体力を返す
	virtual float getHealth()const { return health; }

	virtual entityNS::COLLISION_TYPE getCollisionType() { return collisionType; }


	// ------------------------------------------------------------------------
	// セッタ
	// ------------------------------------------------------------------------

	// 速度を設定
	virtual void setVelocity(VECTOR2 v) { velocity = v; }

	// 速度変化を設定
	virtual void setDeltaV(VECTOR2 dv) { deltaV = dv; }

	// activeを設定
	virtual void setActive(bool a) { active = a; }

	// 体力を設定
	virtual void setHealth(float h) { health = h; }

	// 質量を設定
	virtual void setMass(float m) { mass = m; }

	// 重力定数を設定
	virtual void setGravity(float g) { gravity = g; }

	// 衝突判定の円の半径を設定
	virtual void setCollisionRadius(float r) { radius = r; }

	// 衝突の種類を設定（NONE, CIRCLE, BOX, ROTATED_BOX)
	virtual void setCollisionType(entityNS::COLLISION_TYPE ctype) { collisionType = ctype; }

	// BOX, ROTATD_BOXの衝突判定に使われるRECT構造体を設定
	virtual void setEdge(RECT e) { edge = e; }

	// rotatedBoxReadyを設定
	virtual void setRotaryBoxReady(bool r) { rotatedBoxReady = r; }

	// ------------------------------------------------------------------------
	// その他の関数
	// ------------------------------------------------------------------------

	// Entityを初期化
	// 実行前：*gamePtr = Gameオブジェクトへのポインタ
	// 　　　　width = Imageの幅（ピクセル単位）（０ = テクスチャ全体の幅を使用）
	// 　　　　height = Imageの高さ（ピクセル単位）（０ = テクスチャ全体の高さを使用）
	// 　　　　ncols = テクスチャ内の列数（１からnまで）（０は１と同じ）
	// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
	virtual bool initialize(Game* gamePtr, int width, int height, int ncols, TextureManager* textureM);
	
	// Update
	// 通常、フレームごとに１回呼び出す
	// frameTimeは、移動とアニメーションの速さを制御するために使用
	virtual void update(float frameTime);

	// AI（人工知能）
	// 通常、フレームごとに１回呼び出す
	// 人工知能計算を実行、entは相互作用のために渡される
	virtual void ai(float frameTime, Entity& ent);

	// エンティティをアクティブ化
	virtual void activate();

	// このエンティティともう一方のエンティティの衝突を判定
	// 各エンティティは必ず１種類の衝突を使用します。
	// 衝突の種類を複数必要とする複雑な図形は、
	// 各部分を別個のエンティティとして扱うことにより処理できます。
	// 通常、フレームごとに１回呼び出す
	// 衝突の種類：CIRCLE、BOX、またはROTATED_BOX
	// 実行後：衝突している場合は、true、していない場合はfalseを戻す
	// 　　　　衝突している場合は、collisionVectorを設定
	virtual bool collidesWith(Entity& ent, VECTOR2& collisionVector);

	// このエンティティが指定された矩形の外側にあるか
	// 実行後：矩形の外側にある場合はtrue、それ以外の場合はfalseを戻す
	virtual bool outsideRect(RECT rect);

	// Damage
	// このエンティティが、武器によってダメージを受ける
	// 継承する側のクラスでこの関数をオーバーライド
	virtual void damage(int weapon);

	// Bounce
	// このエンティティがほかのエンティティと衝突した時に跳ねる
	void bounce(VECTOR2& collisionVector, Entity& ent);

	// 重力
	void gravityForce(Entity* other, float frameTime);

};

#endif
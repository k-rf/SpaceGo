#include "entity.h"

// ============================================================================
// 円の衝突判定メソッド
// collision()によって呼び出される、デフォルトの衝突判定メソッド
// 実行後：衝突している場合はtrue、していない場合はfalseを戻す
// 衝突している場合は、collisionVectorを設定
// ============================================================================
bool Entity::collideCircle(Entity& ent, VECTOR2& collisionVector)
{
	// 中心と中心の間の差
	distSquared = *getCenter() - *ent.getCenter();
	distSquared.x = distSquared.x * distSquared.x;  // 差を２乗
	distSquared.y = distSquared.y * distSquared.y;

	// 半径の合計を計算（拡大縮小の倍率を調整）
	sumRadiiSquared = (radius * getScale()) + (ent.radius * ent.getScale());
	sumRadiiSquared *= sumRadiiSquared;  // ２乗する

	// エンティティが衝突している場合
	if(distSquared.x + distSquared.y <= sumRadiiSquared)
	{
		// 衝突ベクトルを設定
		collisionVector = *ent.getCenter() - *getCenter();
		return true;
	}
	return false;  // 衝突していない場合
}

// ============================================================================
// 軸平行境界ボックスの衝突判定メソッド
// collision()によって呼び出される
// 実行後：衝突している場合はtrue、していない場合はfalseを戻す
// 衝突している場合は、collisionVectorを設定
// ============================================================================
bool Entity::collideBox(Entity& ent, VECTOR2& collisionVector)
{
	// どちらかのエンティティがアクティブでない場合、衝突は起こらない
	if(!active || !ent.getActive())
		return false;

	// 軸平行境界ボックスを使って衝突をチェック
	if((getCenterX() + edge.right * getScale() >=
		ent.getCenterX() + ent.getEdge().left * ent.getScale()) &&
		(getCenterX() + edge.left * getScale() <=
		ent.getCenterX() + ent.getEdge().right * ent.getScale()) &&
		(getCenterY() + edge.bottom * getScale() >=
		ent.getCenterY() + ent.getEdge().top * ent.getScale()) &&
		(getCenterY() + edge.top * getScale() <=
		ent.getCenterY() + ent.getEdge().bottom * ent.getScale()))
	{
		// 衝突ベクトルを設定
		collisionVector = *ent.getCenter() - *getCenter();
		return true;
	}
	return false;
}

// ============================================================================
// 回転するボックスの衝突判定メソッド
// collision()によって呼び出される
// 実行後：衝突している場合はtrue、していない場合はfalse
// 　　　　衝突している場合は、collisionVectorを設定
// 分離軸テストを使用して衝突を判定
// 分離軸テスト：
// ２つのボックスは、特定の直線への投影が重なっていなければ衝突していない
// ============================================================================
bool Entity::collideRotatedBox(Entity& ent, VECTOR2& collisionVector)
{
	computeRotatedBox();     // 回転するボックスを準備
	ent.computeRotatedBox(); // 回転するボックスを準備

	if(projectionsOverlap(ent) && ent.projectionsOverlap(*this))
	{
		// 衝突ベクトルを設定
		collisionVector = *ent.getCenter() - *getCenter();
		return true;
	}
	return false;
}

// ============================================================================
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
// ============================================================================
bool Entity::collideRotatedBoxCircle(Entity& ent, VECTOR2& collisionVector)
{
	float min01, min03, max01, max03, center01, center03;
	computeRotatedBox();  // 回転するボックスを準備

	// 円の中心をedge01に投影
	center01 = graphics->Vector2Dot(&edge01, ent.getCenter());
	// 最小値と最大値は中心からの半径
	min01 = center01 - ent.getRadius() * ent.getScale();
	max01 = center01 + ent.getRadius() * ent.getScale();

	if(min01 > edge01Max || max01 < edge01Min)  // 投影が重ならない場合
		return false;  // 衝突の可能性なし

	// 円の中心をedge03に投影
	center03 = graphics->Vector2Dot(&edge03, ent.getCenter());
	// 最小値と最大値は中心からの半径
	min03 = center03 - ent.getRadius() * ent.getScale();
	max03 = center03 + ent.getRadius() * ent.getScale();
	if(min03 > edge03Max || max03 < edge03Min)  // 投影が重ならない場合
		return false;  // 衝突の可能性なし

	// 円の投影がボックスの投影に重なる場合
	// 円が衝突ボックスのボロノイ領域にあるかどうかをチェック

	// 中心がVoronoi0にある場合
	if(center01 < edge01Min && center03 < edge03Min)
		return collideCornerCircle(corners[0], ent, collisionVector);
	// 中心がVoronoi1にある場合
	if(center01 > edge01Max && center03 < edge03Min)
		return collideCornerCircle(corners[1], ent, collisionVector);
	// 中心がVoronoi2にある場合
	if(center01 > edge01Max && center03 > edge03Max)
		return collideCornerCircle(corners[2], ent, collisionVector);
	// 中心がVoronoi3にある場合
	if(center01 < edge01Max && center03 > edge03Max)
		return collideCornerCircle(corners[3], ent, collisionVector);

	// 円がボロノイ領域にない場合、円はボックスの辺と衝突している
	// 衝突ベクトルを設定、単純に、円の中心からボックスの中心までを使用
	collisionVector = *ent.getCenter() - *getCenter();

	return true;
}

// ============================================================================
// ボックスの頂点が円と衝突していないかを、距離のチェックを使用して判定
// collideRotatedBoxCircle()によって呼び出される
// 実行後：衝突している場合はtrue、していない場合はfalseを戻す
// 衝突している場合は、collisionVectorを設定
// ============================================================================
bool Entity::collideCornerCircle(VECTOR2 corner, Entity& ent, VECTOR2& collisionVector)
{
	distSquared = corner - *ent.getCenter();               // 頂点 - 円
	distSquared.x = distSquared.x * distSquared.x;         // 差を２乗
	distSquared.y = distSquared.y * distSquared.y;

	// 半径の合計を計算してから、それを２乗
	sumRadiiSquared = ent.getRadius() * ent.getScale();    // （０ + 円の半径）
	sumRadiiSquared *= sumRadiiSquared;                    // ２乗する

	// 頂点と円が衝突している場合
	if(distSquared.x + distSquared.y <= sumRadiiSquared)
	{
		// 衝突ベクトルを設定
		collisionVector = *ent.getCenter() - corner;
		return true;
	}

	return false;
}

// ============================================================================
// 相手のボックスを、このエンティティのedge01およびedge03に投影
// collideRotatedVox()によって呼び出される
// 実行後：投影が重なっている場合はtrue、それ以外の場合はfalseを戻す
// ============================================================================
bool Entity::projectionsOverlap(Entity& ent)
{
	float projection, min01, max01, min03, max03;

	// 相手のボックスをedge01に投影
	projection = graphics->Vector2Dot(&edge01, ent.getCorner(0));

	// 頂点0を投影
	min01 = projection;
	max01 = projection;

	// 残りの頂点それぞれを処理
	for(int c = 1; c < 4; c++)
	{
		// 頂点をedge01に投影
		projection = graphics->Vector2Dot(&edge01, ent.getCorner(c));
		if(projection < min01)
			min01 = projection;
		else if(projection > max01)
			max01 = projection;
	}

	// 投影が重ならない場合
	if(min01 > edge01Max || max01 < edge01Min)
		return false;  // 衝突の可能性なし

	// 相手のボックスをedge03に投影
	projection = graphics->Vector2Dot(&edge03, ent.getCorner(0));

	// 頂点0を投影
	min03 = projection;
	max03 = projection;

	// 残りの頂点それぞれを処理
	for(int c = 1; c < 4; c++)
	{
		// 頂点をedge03に投影
		projection = graphics->Vector2Dot(&edge03, ent.getCorner(c));
		if(projection < min03)
			min03 = projection;
		else if(projection > max03)
			max03 = projection;
	}

	// 投影が重ならない場合
	if(min03 > edge03Max || max03 < edge03Min)
		return false;  // 衝突の可能性なし
	return true;       // 投影が重なっている場合
}

// ============================================================================
// 回転するボックスの頂点、投影線、投影の最小値と最大値を計算
// 0---1 頂点の番号
// |   |
// 3---2
// ============================================================================
void Entity::computeRotatedBox()
{
	if(rotatedBoxReady)
		return;

	float projection;
	VECTOR2 rotatedX(cos(spriteData.angle), sin(spriteData.angle));
	VECTOR2 rotatedY(-sin(spriteData.angle), cos(spriteData.angle));
	const VECTOR2* center = getCenter();

	corners[0] = *center +
		rotatedX * ((float)edge.left * getScale()) +
		rotatedY * ((float)edge.top * getScale());
	corners[1] = *center +
		rotatedX * ((float)edge.right * getScale()) +
		rotatedY * ((float)edge.top * getScale());
	corners[2] = *center +
		rotatedX * ((float)edge.right * getScale()) +
		rotatedY * ((float)edge.bottom * getScale());
	corners[3] = *center +
		rotatedX * ((float)edge.left * getScale()) +
		rotatedY * ((float)edge.bottom * getScale());

	// corners[0]を基点として使用
	// corners[0]に接する２編を投影線として使用
	edge01 = VECTOR2(corners[1].x - corners[0].x, corners[1].y - corners[0].y);
	graphics->Vector2Normalize(&edge01);
	edge03 = VECTOR2(corners[3].x - corners[0].x, corners[3].y - corners[0].y);
	graphics->Vector2Normalize(&edge03);

	// このエンティティを投影線上に投影したときの最小値と最大値
	projection = graphics->Vector2Dot(&edge01, &corners[0]);
	edge01Min = projection;
	edge01Max = projection;

	// edge01への投影
	projection = graphics->Vector2Dot(&edge01, &corners[1]);
	if(projection < edge01Min)
		edge01Min = projection;
	else if(projection > edge01Max)
		edge01Max = projection;

	// edge03への投影
	projection = graphics->Vector2Dot(&edge03, &corners[0]);
	edge03Min = projection;
	edge03Max = projection;
	projection = graphics->Vector2Dot(&edge03, &corners[3]);
	if(projection < edge03Min)
		edge03Min = projection;
	else if(projection > edge03Max)
		edge03Max = projection;

	rotatedBoxReady = true;
}

// ============================================================================
// コンストラクタ
// ============================================================================
Entity::Entity() : Image()
{
	radius = 1.0;
	edge.left = -1;
	edge.top = -1;
	edge.right = 1;
	edge.bottom = 1;
	mass = 1.0;
	velocity.x = 0.0;
	velocity.y = 0.0;
	deltaV.x = 0.0;
	deltaV.y = 0.0;
	active = true;  // エンティティはアクティブ
	rotatedBoxReady = false;
	collisionType = entityNS::CIRCLE;
	health = 100;
	gravity = entityNS::GRAVITY;
}

// ============================================================================
// Entityを初期化
// 実行前：*gamePtr = Gameオブジェクトへのポインタ
// 　　　　width = Imageの幅（ピクセル単位）（０ = テクスチャ全体の幅を使用）
// 　　　　height = Imageの高さ（ピクセル単位）（０ = テクスチャ全体の高さを使用）
// 　　　　ncols = テクスチャ内の列数（１からnまで）（０は１と同じ）
// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
// ============================================================================
bool Entity::initialize(Game* gamePtr, int width, int height, int ncols, TextureManager* textureM)
{
	input = gamePtr->getInput();  // 入力システム
	return (Image::initialize(gamePtr->getGraphics(), width, height, ncols, textureM));
}

// ============================================================================
// Update
// 通常、フレームごとに１回呼び出す
// frameTimeは、移動とアニメーションの速さを制御するために使用
// ============================================================================
void Entity::update(float frameTime)
{
	velocity += deltaV;
	deltaV.x = 0;
	deltaV.y = 0;
	Image::update(frameTime);
	rotatedBoxReady = false;  // rotatedBoxの衝突判定のため
}

// ============================================================================
// AI（人工知能）
// 通常、フレームごとに１回呼び出す
// 人工知能計算を実行、entは相互作用のために渡される
// ============================================================================
void Entity::ai(float frameTime, Entity& ent) {}

// ============================================================================
// エンティティをアクティブ化
// ============================================================================
void Entity::activate() { active = true; }

// ============================================================================
// このエンティティともう一方のエンティティの衝突を判定
// 各エンティティは必ず１種類の衝突を使用します。
// 衝突の種類を複数必要とする複雑な図形は、
// 各部分を別個のエンティティとして扱うことにより処理できます。
// 通常、フレームごとに１回呼び出す
// 衝突の種類：CIRCLE、BOX、またはROTATED_BOX
// 実行後：衝突している場合は、true、していない場合はfalseを戻す
// 　　　　衝突している場合は、collisionVectorを設定
// ============================================================================
bool Entity::collidesWith(Entity& ent, VECTOR2& collisionVector)
{
	// どちらかのエンティティがアクティブでない場合、衝突は起こらない
	if(!active || !ent.getActive()) 
		return false;

	// 両方のエンティティがCIRCLE衝突である場合
	if(collisionType == entityNS::CIRCLE && ent.getCollisionType() == entityNS::CIRCLE)
		return collideCircle(ent, collisionVector);

	// 両方のエンティティがBOX衝突である場合
	if(collisionType == entityNS::BOX && ent.getCollisionType() == entityNS::BOX)
		return collideBox(ent, collisionVector);

	// 他のすべての組み合わせが使用する分離軸テスト
	// どちらのエンティティもCIRCLE衝突を使用しない場合
	if(collisionType != entityNS::CIRCLE && ent.getCollisionType() != entityNS::CIRCLE)
		return collideRotatedBox(ent, collisionVector);
	// どちらか１つのエンティティが円の場合
	else
	{
		// このエンティティがCIRCLE衝突を使用する場合
		if(collisionType == entityNS::CIRCLE)
			return ent.collideRotatedBoxCircle(*this, collisionVector);
		// もう一方のエンティティがCIRCLE衝突を使用する場合
		else
			return collideRotatedBoxCircle(ent, collisionVector);
	}
	return false;
}

// ============================================================================
// このエンティティが指定された矩形の外側にあるか
// 実行後：矩形の外側にある場合はtrue、それ以外の場合はfalseを戻す
// ============================================================================
bool Entity::outsideRect(RECT rect)
{
	if(spriteData.x + spriteData.width * getScale() < rect.left ||
		spriteData.x > rect.right ||
		spriteData.y + spriteData.height * getScale() < rect.top ||
		spriteData.y > rect.bottom)
		return true;
	return false;
}

// ============================================================================
// Damage
// このエンティティが、武器によってダメージを受ける
// 継承する側のクラスでこの関数をオーバーライド
// ============================================================================
void Entity::damage(int weapon) {}

// ============================================================================
// Bounce
// このエンティティが他のエンティティと衝突した時に跳ねる
// ============================================================================
void Entity::bounce(VECTOR2& collisionVector, Entity& ent) {}

// ============================================================================
// 重力
// ============================================================================
void Entity::gravityForce(Entity* other, float frameTime) {}
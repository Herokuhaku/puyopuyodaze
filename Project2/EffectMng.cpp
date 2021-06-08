#include "EffectMng.h"
#include <EffekseerForDXLib.h>
#include "Scene/SceneMng.h"
EffectMng* EffectMng::sInstance = nullptr;

bool EffectMng::Init(Vector2&& size)
{
	if (Effekseer_Init(8000) == -1)
	{
		return false;
	}
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	Effekseer_Set2DSetting(size.x,size.y);
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);

	return true;
}

void EffectMng::Update(void)
{
	UpdateEffekseer2D();
	if (playlist_.size() >= 1) {
		auto itr = std::remove_if(playlist_.begin(), playlist_.end(), [&](int image) {return IsEffekseer2DEffectPlaying(image)==-1; });
		playlist_.erase(itr, playlist_.end());
	}
}

void EffectMng::Draw(void)
{
	DrawEffekseer2D();
}

bool EffectMng::StopEffectAll(void)
{
	for (auto& image : playlist_)
	{
		StopEffekseer2DEffect(image);
	}
	return true;
}

bool EffectMng::PlayEffect(std::string key, const Vector2& pos)
{
	// Ä¶‚·‚é‚à‚Ì‚Ì‘IoBGetImage‚Åkey‚ğæ“¾‚µ‚Äplaylist‚É“Š‚°‚é
	playlist_.push_front(PlayEffekseer2DEffect(GetImage(key)));
	SetPosPlayingEffekseer2DEffect(*playlist_.begin(),static_cast<float>(pos.x), static_cast<float>(pos.y),0);
	
	return true;
}
int EffectMng::GetImage(std::string key)
{
	images_.try_emplace(key,LoadEffekseerEffect(("effect/"+key+".efk").c_str(),2.0f));
	return images_[key];
}
EffectMng::EffectMng()
{
}

EffectMng::~EffectMng()
{
}

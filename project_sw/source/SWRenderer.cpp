#include "SWRenderer.h"
#include "SWGameObject.h"
#include "SWGameContext.h"
#include "SWGameScene.h"

void SWRenderer::onAdded()
{
	SWGameScene* scene = SW_GC.getScene();
	scene->m_renderers.push_back( this );
}

void SWRenderer::onRemoved()
{
	SWGameScene* scene = SW_GC.getScene();
	scene->m_renderers.remove( this );
}
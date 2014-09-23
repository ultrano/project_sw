#include "SWRenderer.h"
#include "SWGameObject.h"
#include "SWGameContext.h"
#include "SWGameScene.h"

SWRenderer::SWRenderer()
{

}

SWRenderer::SWRenderer( factory_constructor )
{

}

SWRenderer::~SWRenderer()
{

}

void SWRenderer::onAwake()
{
	SWGameScene* scene = SW_GC.getScene();
	scene->m_renderers.push_back( this );
}

void SWRenderer::onRemove()
{
	SWGameScene* scene = SW_GC.getScene();
	scene->m_renderers.remove( this );
}

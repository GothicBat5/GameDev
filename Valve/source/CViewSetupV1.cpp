#ifndef VIEW_SHAREDV1_H
#define VIEW_SHAREDV1_H

#ifdef _WIN32
#pragma once
#endif
class CViewSetupV1
{
public:
	CViewSetupV1()
	{
		m_bForceAspectRatio1To1 = false;
		m_bRenderToSubrectOfLargerScreen = false;
		bForceClearWholeRenderTarget = false;
		m_bUseRenderTargetAspectRatio = false;
	}

	int	context;			
	int	x;					
	int	y;					
	int	width;				
	int	height;				
	bool clearColor;			
	bool clearDepth;			
	bool bForceClearWholeRenderTarget;
	bool m_bOrtho;			
	float m_OrthoLeft;		
	float	m_OrthoTop;
	float	m_OrthoRight;
	float	m_OrthoBottom;
	float	fov;				
	float	fovViewmodel;		
	Vector origin;							
	Vector m_vUnreflectedOrigin;						
	QAngle angles;				
	float zNear;			
	float	zFar;			
	float	zNearViewmodel;		
	float	zFarViewmodel;		
	bool m_bForceAspectRatio1To1;
	bool m_bRenderToSubrectOfLargerScreen;
	bool		m_bUseRenderTargetAspectRatio;
};

#endif // VIEW_SHAREDV1_H
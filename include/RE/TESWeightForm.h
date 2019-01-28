#pragma once

#include "skse64/GameFormComponents.h"  // BaseFormComponent


namespace RE
{
	class TESWeightForm : public BaseFormComponent
	{
	public:
		virtual ~TESWeightForm();	// 00

		// override (BaseFormComponent)
		virtual void	Init() override;									// 01
		virtual void	ReleaseRefs() override;								// 02 - { return; }
		virtual void	CopyFromBase(BaseFormComponent* a_rhs) override;	// 03


		// members
		float	weight;	// 08
		UInt32	pad0C;	// 0C
	};
	STATIC_ASSERT(sizeof(TESWeightForm) == 0x10);
}
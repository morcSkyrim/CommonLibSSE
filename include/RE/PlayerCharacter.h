#pragma once

#include <ctime>

#include "RE/BGSPerkRankArray.h"
#include "RE/BSPointerHandle.h"
#include "RE/BSSoundHandle.h"
#include "RE/BSTArray.h"
#include "RE/BSTEvent.h"
#include "RE/BSTHashMap.h"
#include "RE/BSTList.h"
#include "RE/BSTSmartPointer.h"
#include "RE/BSTTuple.h"
#include "RE/Character.h"
#include "RE/FormTypes.h"
#include "RE/NiPoint3.h"
#include "RE/NiSmartPointer.h"
#include "RE/NiTMap.h"
#include "RE/PositionPlayerEvent.h"


namespace RE
{
	class Actor;
	class BGSLocation;
	class BSFadeNode;
	class ImageSpaceModifierInstanceDOF;
	class InventoryEntryData;
	class MenuModeChangeEvent;
	class MenuOpenCloseEvent;
	class ObjectListItem;
	class TESObjectREFR;
	class TintMask;
	class UserEventEnabledEvent;
	struct BGSActorCellEvent;
	struct BGSActorDeathEvent;
	struct PositionPlayerEvent;
	struct TESTrackedStatsEvent;


	class PlayerCharacter :
		public Character,							// 000
		public BSTEventSource<BGSActorCellEvent>,	// 2D0
		public BSTEventSource<BGSActorDeathEvent>,	// 328
		public BSTEventSource<PositionPlayerEvent>,	// 380
		public BSTEventSink<MenuOpenCloseEvent>,	// 2B0
		public BSTEventSink<MenuModeChangeEvent>,	// 2B8
		public BSTEventSink<UserEventEnabledEvent>,	// 2C0
		public BSTEventSink<TESTrackedStatsEvent>	// 2C8
	{
	public:
		inline static const void* RTTI = RTTI_PlayerCharacter;


		enum { kTypeID = FormType::ActorCharacter };


		enum class EventType : UInt32
		{
			kThief = 3,
			kContainer = 5,
			kDeadBody = 6
		};


		enum class FlagBDB : UInt8
		{
			kNone = 0,
			kIsInCombat = 1 << 5
		};


		struct CrimeGold
		{
			float	violentCur;			// 00
			float	nonViolentCur;		// 04
			float	nonViolentInfamy;	// 08
			float	violentInfamy;		// 0C
		};
		STATIC_ASSERT(sizeof(CrimeGold) == 0x10);


		struct StolenItemValue
		{
			SInt32	unwitnessed;	// 0
			SInt32	witnessed;		// 4
		};
		STATIC_ASSERT(sizeof(StolenItemValue) == 0x8);


		struct DataUnk510
		{
			UInt32	unk00;	// 00
			UInt32	unk04;	// 04
			UInt32	unk08;	// 08
			UInt32	unk0C;	// 0C
		};
		STATIC_ASSERT(sizeof(DataUnk510) == 0x10);


		struct Data600
		{
			UInt64	unk00;	// 00
			UInt64	unk08;	// 08
		};
		STATIC_ASSERT(sizeof(Data600) == 0x10);


		struct Data7DC
		{
			struct Data
			{
				float		unk0;	// 0
				RefHandle	unk4;	// 4
				UInt32		unk8;	// 8
			};
			STATIC_ASSERT(sizeof(Data) == 0xC);


			Data	data[15];	// 00
			UInt32	count;		// B4
		};
		STATIC_ASSERT(sizeof(Data7DC) == 0xB8);


		struct Data928
		{
			BSTArray<UnkValue>	unk00;	// 00
			BSTArray<UnkValue>	unk18;	// 18
			UInt64				unk30;	// 30
			UInt64				unk38;	// 38
			UInt64				unk40;	// 40
		};
		STATIC_ASSERT(sizeof(Data928) == 0x48);


		struct PlayerSkills
		{
			struct Data
			{
				struct Skills
				{
					enum Skill : UInt32
					{
						kOneHanded = 0,
						kTwoHanded = 1,
						kArchery = 2,
						kBlock = 3,
						kSmithing = 4,
						kHeavyArmor = 5,
						kLightArmor = 6,
						kPickpocket = 7,
						kLockpicking = 8,
						kSneak = 9,
						kAlchemy = 10,
						kSpeech = 11,
						kAlteration = 12,
						kConjuration = 13,
						kDestruction = 14,
						kIllusion = 15,
						kRestoration = 16,
						kEnchanting = 17,
						kTotal
					};
				};
				using Skill = Skills::Skill;


				struct SkillData
				{
					float	level;			// 0
					float	xp;				// 4
					float	levelThreshold;	// 8
				};
				STATIC_ASSERT(sizeof(SkillData) == 0xC);


				float		xp;								// 000
				float		levelThreshold;					// 004
				SkillData	skills[Skill::kTotal];			// 008
				UInt32		legendaryLevels[Skill::kTotal];	// 0E0
			};
			STATIC_ASSERT(sizeof(Data) == 0x128);


			void AdvanceLevel(bool a_addThreshold);


			// members
			Data* data;	// 0
		};
		STATIC_ASSERT(sizeof(PlayerSkills) == 0x8);


		virtual ~PlayerCharacter();													// 000

		// add
		virtual void	Unk_12A(void);												// 12A
		virtual SInt32	GetCrimeGoldViolent(const TESFaction* a_faction) const;		// 12B
		virtual SInt32	GetCrimeGoldNonViolent(const TESFaction* a_faction) const;	// 12C
		virtual void	Unk_12D(void);												// 12D
		virtual void	Unk_12E(void);												// 12E - { return 0; }

		static PlayerCharacter*	GetSingleton();
		static UInt32			GetPickpocketChance(float a_playerSkill, float a_targetSkill, UInt32 a_totalValue, float a_itemWeight, Actor* a_player, Actor* a_target, bool a_isDetected, TESForm* a_item);

		Actor*						GetActorInFavorState() const;
		float						GetArmorValue(InventoryEntryData* a_form);
		float						GetDamage(InventoryEntryData* a_form);
		NiPointer<TESObjectREFR>	GetGrabbedRef();
		UInt32						GetNumTints(UInt32 a_tintType);
		TintMask*					GetOverlayTintMask(TintMask* a_original);
		BSTArray<TintMask*>&		GetTintList();
		TintMask*					GetTintMask(UInt32 a_tintType, UInt32 a_index);
		void						PlayPickupEvent(TESForm* a_item, TESForm* a_containerOwner, TESObjectREFR* a_containerRef, EventType a_eventType);
		void						SetCollision(bool a_enable);
		void						StartActivation();
		bool						TryToPickPocket(Actor* a_target, InventoryEntryData* a_entry, UInt32 a_numItems, bool a_arg4 = true);


		// members
		UInt32											unk3D8;						// 3D8
		UInt32											unk3DC;						// 3DC
		BSTHashMap<const TESFaction*, CrimeGold>		crimeGoldMap;				// 3E0
		BSTHashMap<const TESFaction*, StolenItemValue>	stolenItemValueMap;			// 410
		ObjectRefHandle									commandWaitMarker;			// 440
		UInt32											unk444;						// 444
		BSTHashMap<const TESFaction*, UnkValue>			factionOwnerFriendsMap;		// 448
		NiPoint3										lastKnownGoodPosition;		// 478
		float											unk484;						// 484
		float											unk488;						// 488
		float											unk48C;						// 48C
		float											unk490;						// 490
		float											unk494;						// 494
		float											unk498;						// 498
		float											unk49C;						// 49C
		UInt64											unk4A0;						// 4A0
		UInt64											unk4A8;						// 4A8
		BSTArray<BGSPerkRankArray::Entry*>				addedPerks;					// 4B0
		BSTArray<BGSPerk*>								perks;						// 4C8
		BSTArray<BGSPerk*>								standingStonePerks;			// 4E0
		BSTArray<ObjectRefHandle>						currentMapMarkers;			// 4F8
		BSTArray<BSTTuple<NiPoint3, AITimeStamp>>		velocityArray;				// 510
		BSTArray<UInt64>								unk528;						// 528
		BSTArray<void*>									imageSpaceModifierAnims1;	// 540
		BSTArray<void*>									imageSpaceModifierAnims2;	// 558
		BSSimpleList<void*>								questLog;					// 570
		BSTArray<void*>									listObjectives;				// 580
		BSTHashMap<UnkKey, UnkValue>					unk598;						// 598
		BSTHashMap<UnkKey, UnkValue>					unk5C8;						// 5C8
		UInt32											unk5F8;						// 5F8
		UInt32											pad5FC;						// 5FC
		BSSimpleList<Data600*>*							unk600;						// 600 - actor distance list?
		NiTMap<UInt32, UInt8>							randomDoorSpaceMap;			// 608
		TESWorldSpace*									cachedWorldSpace;			// 628
		NiPoint3										exteriorPosition;			// 630
		UInt32											unk63C;						// 63C
		UInt64											unk640;						// 640
		UInt64											unk648;						// 648
		float											unk650;						// 650
		float											unk654;						// 654
		float											unk658;						// 658
		float											unk65C;						// 65C
		float											unk660;						// 660
		float											unk664;						// 664
		UInt64											unk668;						// 668
		UInt64											unk670;						// 670
		RefHandle										unk678;						// 678
		RefHandle										unk67C;						// 67C
		UInt8											unk680;						// 680
		UInt8											unk681;						// 681
		UInt8											unk682;						// 682
		UInt8											pad683;						// 683
		UInt32											unk684;						// 684
		BSSoundHandle									unk688;						// 688
		BSSoundHandle									unk694;						// 694
		BSSoundHandle									unk6A0;						// 6A0
		UInt32											pad6AC;						// 6AC
		TESPackage*										inGameDialoguePackage;		// 6B0
		UInt32											unk6B8;						// 6B8
		UInt32											unk6BC;						// 6BC
		UInt64											unk6C0;						// 6C0
		UInt32											unk6C8;						// 6C8
		float											unk6CC;						// 6CC
		UInt32											unk6D0;						// 6D0
		float											unk6D4;						// 6D4
		float											unk6D8;						// 6D8
		UInt32											pad6DC;						// 6DC
		ImageSpaceModifierInstanceDOF*					unk6E0;						// 6E0
		ImageSpaceModifierInstanceDOF*					unk6E8;						// 6E8
		ImageSpaceModifierInstanceDOF*					unk6F0;						// 6F0
		UInt32											unk6F8;						// 6F8
		UInt32											unk6FC;						// 6FC
		UInt64											unk700;						// 700
		UInt64											unk708;						// 708
		UInt64											unk710;						// 710
		UInt64											unk718;						// 718
		UInt32											unk720;						// 720
		UInt32											pad724;						// 724
		void*											unk728;						// 728 - smart ptr
		UInt8											unk730[0xA0];				// 730
		UInt32											unk7D0;						// 7D0
		UInt32											unk7D4;						// 7D4
		RefHandle										unk7D8;						// 7D8
		Data7DC											unk7DC;						// 7DC
		RefHandle										actorInFavorState;			// 894
		BSTSmallArray<void*, 4>							unk898;						// 898
		ObjectRefHandle									grabbedObject;				// 8C8
		float											unk8CC;						// 8CC - related to grabbed item
		UInt32											unk8D0;						// 8D0
		float											unk8D4;						// 8D4
		UInt64											unk8D8;						// 8D8
		UInt32											unk8E0;						// 8E0
		UInt32											unk8E4;						// 8E4
		BSTSmartPointer<Biped>							largeBiped;					// 8E8
		NiPointer<BSFadeNode>							firstPerson3D;				// 8F0
		float											unk8F8;						// 8F8
		UInt32											unk8FC;						// 8FC
		float											unk900;						// 900
		UInt32											pad904;						// 904
		UInt64											unk908;						// 908
		UInt32											unk910;						// 910
		ActorHandle										lastRiddenMount;			// 914
		ActorHandle										lightTarget;				// 918
		UInt32											unk91C;						// 91C
		UInt64											unk920;						// 920
		Data928*										unk928;						// 928
		UInt32											timesTrained;				// 930
		UInt32											unk934;						// 934
		UInt64											unk938;						// 938
		UInt64											unk940;						// 940
		UInt64											unk948;						// 948
		UInt64											unk950;						// 950
		UInt64											unk958;						// 958
		UInt32											unk960;						// 960
		UInt32											unk964;						// 964
		AlchemyItem*									pendingPoison;				// 968
		UInt32											tickCount;					// 970
		UInt32											unk974;						// 974
		UInt64											unk978;						// 978
		std::time_t										unk980;						// 980
		TESForm*										lastKnownGoodLocation;		// 988
		UInt32											unk990;						// 990
		UInt32											unk994;						// 994
		void*											unk998;						// 998 - smart ptr
		void*											unk9A0;						// 9A0 - smart ptr
		UInt64											unk9A8;						// 9A8
		PlayerSkills*									skills;						// 9B0
		RefHandle										targetHandle;				// 9B8
		RefHandle										unk9BC;						// 9BC
		UInt64											unk9C0;						// 9C0
		NiPointer<BSFadeNode>							unk9C8;						// 9C8
		void*											unk9D0;						// 9D0
		BSTArray<ActorHandle>							actorsToDisplayOnTheHUDArray; 	// 9D8
		UInt64											unk9F0;						// 9F0
		UInt64											unk9F8;						// 9F8
		AlchemyItem*									tempPoison;					// A00
		UInt32											teammateCount;				// A08
		UInt32											unkA0C;						// A0C
		UInt64											unkA10;						// A10
		float											animTimeout;				// A18
		UInt32											unkA1C;						// A1C
		UInt8											unkA20[0xA0];				// A20
		UInt32											unkAC0;						// AC0
		UInt32											unkAC4;						// AC4
		BGSLocation*									currentLocation;			// AC8
		float											unkAD0;						// AD0
		UInt32											unkAD4;						// AD4
		UInt32											unkAD8;						// AD8
		UInt32											unkADC;						// ADC
		UInt64											unkAE0;						// AE0
		SInt32											unkAE8;						// AE8
		UInt32											unkAEC;						// AEC
		UInt32											unkAF0;						// AF0
		UInt32											unkAF4;						// AF4
		SInt32											difficulty;					// AF8
		UInt32											unkAFC;						// AFC
		UInt8											unkB00;						// B00
		UInt8											numPerkPoints;				// B01
		UInt8											unkB02;						// B02 - flags
		UInt8											padB03;						// B03
		UInt32											unkB04;						// B04
		void*											unkB08;						// B08
		BSTArray<TintMask*>								tintMasks;					// B10
		BSTArray<TintMask*>*							overlayTintMasks;			// B28
		BGSTextureSet*									texSetB30;					// B30
		TESRace*										baseRace;					// B38 - untransformed race
		TESRace*										race2;						// B40
		SInt32											unkB48;						// B48
		UInt32											padB4C;						// B4C
		BSTArray<UInt64>								unkB50;						// B50
		UInt64											unkB68;						// B68
		UInt64											unkB70;						// B70
		UInt64											unkB78;						// B78
		UInt64											unkB80;						// B80
		SInt32											unkB88;						// B88
		UInt32											padB8C;						// B8C
		UInt64											unkB90;						// B90
		UInt64											unkB98;						// B98
		BSTSmallArray<void*, 4>							unkBA0;						// BA0
		UInt64											unkBD0;						// BD0
		UInt8											unkBD8;						// BD8
		UInt8											unkBD9;						// BD9
		UInt8											unkBDA;						// BDA
		FlagBDB											unkBDB;						// BDB
		UInt8											unkBDC;						// BDC
		UInt8											unkBDD;						// BDD
		UInt16											padBDE;						// BDE
	};
	STATIC_ASSERT(sizeof(PlayerCharacter) == 0xBE0);
}

//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_SECTION_H
#define MRW_MODEL_SECTION_H

#include <vector>
#include <regex>
#include <functional>
#include <type_traits>

#include <QDomElement>

#include <util/constantenumerator.h>
#include <util/method.h>
#include <util/stringutil.h>
#include <model/assemblypart.h>
#include <model/module.h>
#include <model/device.h>
#include <model/position.h>

namespace mrw::model
{
	class ModelRailway;
	class Region;
	class SectionModule;
	class Signal;

	enum class SectionState : int
	{
		/**
		 * The Section is in free/available state.
		 *
		 * <img src="Rail____IS_RUF.jpg" width="100"/>
		 */
		FREE = 0,

		/**
		 * The section is reserved for shunting.
		 *
		 * <img src="Rail____IS_RUS.jpg" width="100"/>
		 */
		SHUNTING,

		/**
		 * The section is reserved for a tour.
		 *
		 * <img src="Rail____IS_RUT.jpg" width="100"/>
		 */
		TOUR,

		/**
		 * The section is occupied by a train.
		 *
		 * <img src="Rail____IS_RUO.jpg" width="100"/>
		 */
		OCCUPIED,

		/**
		 * The section has left by a train and is still reserved.
		 *
		 * <img src="Rail____IS_RUP.jpg" width="100"/>
		 */
		PASSED
	};

	/**
	 * A Section manages exactly one occupation state for a RailPart
	 * collection. It may also manage several Signal instances. The Section
	 * is controlled by a SectionModule.
	 */
	class Section : public Device, public Position, public mrw::util::String
	{
		friend class Region;

		static const std::regex                                   path_regex;
		static const mrw::util::ConstantEnumerator<SectionState>  state_map;

		const QString                section_name;
		ModelRailway        *        model              = nullptr;
		Region           *           section_region     = nullptr;
		Controller         *         section_controller = nullptr;
		SectionModule        *       section_module     = nullptr;
		SectionState                 section_state      = SectionState::FREE;
		bool                         section_enabled    = false;
		bool                         section_occupied   = false;
		std::vector<AssemblyPart *>  assembly_parts;
		std::vector<Signal *>        forward_signals;
		std::vector<Signal *>        backward_signals;

	public:
		explicit Section(
			ModelRailway     *    model_railway,
			Region        *       region,
			const QDomElement  &  element);
		virtual ~Section();

		// Implementations from Device
		const QString    &   name()         const noexcept override;
		bool                 isUnlockable() const noexcept override;
		Controller     *     controller()   const noexcept override;
		mrw::can::MrwMessage configMsg(const unsigned pin) const override;

		// Implementations from Position
		virtual QString key() const noexcept override;

		// Implementations from mrw::util::String
		QString         toString() const override;

		/**
		 * This method validates if all references are configured correctly.
		 *
		 * @return True if every configuration is fine.
		 */
		bool valid() const noexcept;

		/**
		 * This method sets the occupation mode of the section. This method
		 * call should occur on mrw::can::MrwMessage GETRBS mrw::can::Command.
		 *
		 * @param input True if the section is occupied.
		 */
		void setOccupation(const bool input = true) noexcept;

		/**
		 * This method returns the occupation state of this Section.
		 *
		 * @return True if this Section is occupied by a train.
		 */
		bool occupation() const noexcept;

		/**
		 * This method marks this Section as enabled.
		 *
		 * @note This is only for simuation purposes and does not have any
		 * influence on driving duty.
		 *
		 * @param enable The enable flag.
		 * @see mrw::can::Command::SETRON
		 */
		void enable(const bool enable = true) noexcept;

		/**
		 * This method marks this Section as disabled.
		 *
		 * @note This is only for simuation purposes and does not have any
		 * influence on driving duty.
		 *
		 * @param disable The disable flag.
		 * @see mrw::can::Command::SETROF
		 */
		void disable(const bool disable = true) noexcept;

		/**
		 * This method returns the enabled state of this Section.
		 *
		 * @note This is only for simuation purposes and does not have any
		 * influence on driving duty.
		 *
		 * @returns True if this Section is enabled.
		 * @see mrw::can::Command::SETROF
		 * @see mrw::can::Command::SETRON
		 */
		bool enabled() const noexcept;

		/**
		 * This method returns the SectionState of this Section.
		 *
		 * @note If this Section is occupied by a train it returns always
		 * SectionState::OCCUPIED regardless on the real SectionState.
		 *
		 * @return The SectionState of this Section.
		 */
		SectionState state() const noexcept;

		/**
		 * This method sets a new SectionState to this Section. This occurs
		 * during routing in case of SectionState::SHUNTING or
		 * SectionState::TOUR or after a train left the Section then it is
		 * SectionState::FREE in case of automatic deallocation or
		 * SectionState::PASSED to mark the Section reserved.
		 *
		 * @param input The new SectionState to set.
		 */
		void setState(const SectionState input) noexcept;

		/**
		 * This method returns the nth RailPart. It is not ID-based but index
		 * based and is used for linking the RailPart elements after the
		 * model has loaded into memory.
		 *
		 * @param index The zero based index of connected RailPart.
		 * @return The found RailPart instance.
		 */
		AssemblyPart * assemblyPart(const int index) const;

		/**
		 * This method returns the amount of AssemblyPart elements contained in
		 * this Section.
		 *
		 * @return The contained AssemblyPart count.
		 */
		size_t assemblyPartCount() const noexcept;

		/**
		 * This method returns the controlling SectionModule.
		 *
		 * @return The controlling SectionModule.
		 */
		SectionModule * module() const noexcept;

		/**
		 * This method returns the Region to which this Section is belonging to.
		 *
		 * @return The Region of this Section.
		 */
		Region * region() const noexcept;

		/**
		 * This template class returns all AssemblyPart elements of the given
		 * type T. The found elements are stored into the given std::vector.
		 *
		 * @param result The result vector collecting the AssembyPart elements
		 * of type T.
		 * @param guard A labmda to fine select if the type T should added to
		 * the result vector.
		 */
		template <class T> constexpr void parts(
			std::vector<T *> & result,
			std::function<bool(const T * part)> guard = &mrw::util::Method::always<T>) const noexcept
		{
			for (AssemblyPart * part : assembly_parts)
			{
				T * element = dynamic_cast<T *>(part);

				if ((element != nullptr) && guard(element))
				{
					result.push_back(element);
				}
			}
		}

		/**
		 * This method unreserves all RailPart elements and sets the
		 * SectionState to SectionState::FREE.
		 *
		 * @see RailPart
		 * @see SectionState
		 */
		void free() noexcept;

		/**
		 * This method returns true if the internal SectionState is
		 * SectionState::FREE regardless of the occupied state. This is
		 * necessary to unlock the Section and allowing relais state turning.
		 *
		 * @return True if the SectionState is real SectionState::FREE.
		 */
		bool isFree() const noexcept;

		/**
		 * This method returns true if the internal SectionState is
		 * SectionState::TOUR regardless of the occupied state. This is
		 * necessary to unlock the Section and allowing relais state turning.
		 *
		 * @return True if the SectionState is real SectionState::TOUR.
		 */
		bool isFlankRelevant() const noexcept;

		/**
		 * This method returns true if any of the contained RailPart elements
		 * are reserved.
		 *
		 * @note In case of a containing switch it is possible that some
		 * RailPart elements are not reserved. The Section is not allocatable
		 * for routing but non reserved RailPart elements are drawn as free
		 * in a GUI.
		 *
		 * @return True if any containing RailPart is reserved.
		 */
		bool anyReserved() const noexcept;

		/**
		 * This method returns the clear text QString of the SectionState this
		 * Section is set to.
		 *
		 * @param state The SectionState enumeration to translate.
		 * @return The QString clear text of the given SectionState enumeration.
		 */
		static QString get(const SectionState & state) noexcept;

		/**
		 * This method returns all signals in this Section in a given view
		 * direction. The resulting Signal elements are sorted with the most
		 * important Signal first. So if the first Signal is a main signal
		 * it marks an end of a block.
		 *
		 * @param view The direction related to the count direction.
		 * @return The sorted Signal vector related to the view direction.
		 * @see Signal::less()
		 */
		const std::vector<Signal *> & getSignals(const bool view) const noexcept;

	private:
		void            add(AssemblyPart * rail_part) noexcept;
		void            link() noexcept;
		SectionModule * resolve(const std::string & path) noexcept;

	};
}

#endif

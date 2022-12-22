//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
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
#include <util/stringutil.h>
#include <model/module.h>
#include <model/device.h>
#include <model/position.h>

namespace mrw::model
{
	class ModelRailway;
	class Region;
	class SectionModule;
	class AssemblyPart;
	class Signal;

	enum SectionState
	{
		/**
		 * The Section is in free/available state.
		 *
		 * @image html Rail____I_RUF.jpg width=25%
		 */
		FREE = 0,

		/**
		 * The section is reserved for shunting.
		 *
		 * @image html Rail____I_RUS.jpg width=25%
		 */
		SHUNTING,

		/**
		 * The section is reserved for a tour.
		 *
		 * @image html Rail____I_RUT.jpg width=25%
		 */
		TOUR,

		/**
		 * The section is occupied by a train.
		 *
		 * @image html Rail____I_RUO.jpg width=25%
		 */
		OCCUPIED,

		/**
		 * The section has left by a train and is still reserved.
		 *
		 * @image html Rail____I_RUP.jpg width=25%
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
		SectionState                 section_state      = FREE;
		bool                         occupied           = false;
		std::vector<AssemblyPart *>  assembly_parts;
		std::vector<Signal *>        forward_signals;
		std::vector<Signal *>        backward_signals;

	public:
		explicit Section(
			ModelRailway     *    model_railway,
			Region        *       region,
			const QDomElement  &  element);
		virtual ~Section();

		/**
		 * This method validates if all references are configured correctly.
		 *
		 * @return True if every configuration is fine.
		 */
		bool valid() const;

		const QString & name() const override;
		bool            notUnlockable() const override;
		QString         toString() const override;
		virtual QString key() const override;

		/**
		 * This method sets the occupation mode of the section. This method
		 * call should occur on mrw::can::MrwMessage GETRBS mrw::can::Command.
		 *
		 * @param input True if the section is occupied.
		 */
		void setOccupation(const bool input = true);

		/**
		 * This method returns the occupation state of this Section.
		 *
		 * @return True if this Section is occupied by a train.
		 */
		bool occupation() const;

		/**
		 * This method returns the SectionState of this Section.
		 *
		 * @note If this Section is occupied by a train it returns always
		 * SectionState::OCCUPIED regardless on the real SectionState.
		 *
		 * @return The SectionState of this Section.
		 */
		SectionState state() const;

		/**
		 * This method sets a new SectionState to this Section. This occurs
		 * during routing in case of SectionState::SHUNTING or
		 * SectionState::TOUR or after a train left the Section then it is
		 * SectionState::FREE in case of automatic deallocation or
		 * SectionState::PASSED to mark the Section reserved.
		 *
		 * @param input The new SectionState to set.
		 */
		void setState(const SectionState input);

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
		size_t assemblyPartCount() const;

		/**
		 * This method returns the controlling SectionModule.
		 *
		 * @return The controlling SectionModule.
		 */
		SectionModule * module() const;

		/**
		 * This method returns the CAN Controller to which the controlling
		 * SectionModule is connected to.
		 *
		 * @return The CAN Controller of the controlling SectionModule.
		 */
		Controller * controller() const override;

		/**
		 * This method returns the Region to which this Section is belonging to.
		 *
		 * @return The Region of this Section.
		 */
		Region * region() const;

		/**
		 * This template class returns all AssemblyPart elements of the given
		 * type T. The found elements are stored into the given std::vector.
		 *
		 * @param result The result vector collecting the AssembyPart elements
		 * of type T.
		 * @param guard A labmda to fine select if the type T should added to
		 * the result vector.
		 */
		template <class T> void parts(
			std::vector<T *> & result,
			std::function<bool(const T * part)> guard = [](const T * part)
		{
			(void)part;

			return true;
		}) const
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
		void free();

		/**
		 * This method returns true if the internal SectionState is
		 * SectionState::FREE regardless of the occupied state. This is
		 * necessary to unlock the Section and allowing relais state turning.
		 *
		 * @return True if the SectionState is real SectionState::FREE.
		 */
		bool isFree() const;

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
		bool anyReserved() const;

		/**
		 * This method returns the clear text QString of the SectionState this
		 * Section is set to.
		 *
		 * @param state The SectionState enumeration to translate.
		 * @return The QString clear text of the given SectionState enumeration.
		 */
		static QString get(const SectionState & state);

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
		const std::vector<Signal *> & getSignals(const bool view);

	private:
		void            add(AssemblyPart * rail_part);
		void            link();
		void            findSignalPair();
		SectionModule * resolve(const std::string & path);

	};
}

#endif // MRW_MODEL_SECTION_H

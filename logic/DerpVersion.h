/* Copyright 2013 MultiMC Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <QAbstractListModel>

#include <QString>
#include <QList>
#include <memory>

#include "DerpLibrary.h"

class DerpInstance;

class DerpVersion : public QAbstractListModel
{
	Q_OBJECT
public:
	explicit DerpVersion(DerpInstance *instance, QObject *parent = 0);

	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation,
								int role = Qt::DisplayRole) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;

	bool reload(QWidget *widgetParent);
	void clear();

	void dump() const;

public:
	QList<std::shared_ptr<DerpLibrary>> getActiveNormalLibs();
	QList<std::shared_ptr<DerpLibrary>> getActiveNativeLibs();

	static std::shared_ptr<DerpVersion> fromJson(const QJsonObject &obj);

	// data members
public:
	/// the ID - determines which jar to use! ACTUALLY IMPORTANT!
	QString id;
	/// Last updated time - as a string
	QString time;
	/// Release time - as a string
	QString releaseTime;
	/// Release type - "release" or "snapshot"
	QString type;
	/// Assets type - "legacy" or a version ID
	QString assets;
	/**
	 * DEPRECATED: Old versions of the new vanilla launcher used this
	 * ex: "username_session_version"
	 */
	QString processArguments;
	/**
	 * arguments that should be used for launching minecraft
	 *
	 * ex: "--username ${auth_player_name} --session ${auth_session}
	 *      --version ${version_name} --gameDir ${game_directory} --assetsDir ${game_assets}"
	 */
	QString minecraftArguments;
	/**
	 * the minimum launcher version required by this version ... current is 4 (at point of
	 * writing)
	 */
	int minimumLauncherVersion = 0xDEADBEEF;
	/**
	 * The main class to load first
	 */
	QString mainClass;

	/// the list of libs - both active and inactive, native and java
	QList<std::shared_ptr<DerpLibrary>> libraries;

	/*
	FIXME: add support for those rules here? Looks like a pile of quick hacks to me though.

	"rules": [
		{
		"action": "allow"
		},
		{
		"action": "disallow",
		"os": {
			"name": "osx",
			"version": "^10\\.5\\.\\d$"
		}
		}
	],
	"incompatibilityReason": "There is a bug in LWJGL which makes it incompatible with OSX
	10.5.8. Please go to New Profile and use 1.5.2 for now. Sorry!"
	}
	*/
	// QList<Rule> rules;

private:
	DerpInstance *m_instance;
};

QDebug operator<<(QDebug &dbg, const DerpVersion *version);
QDebug operator<<(QDebug &dbg, const DerpLibrary *library);

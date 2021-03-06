/*
  Copyright (C) 2014 Michal Kosciesza <michal@mkiol.net>

  This file is part of Kaktus.

  Kaktus is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Kaktus is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Kaktus.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QDir>
#include <QByteArray>
#include <QVariant>
#include <QList>
#include <QMap>
#include <QDateTime>

#include "settings.h"

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    static const int dashboardsLimit = 100;
    static const int tabsLimit = 100;
    static const int streamLimit = 100;
    static const int entriesLimit = 100;

    struct StreamModuleTab {
        QString streamId;
        QString moduleId;
        QString tabId;
        int date;
    };

    struct Dashboard {
        QString id;
        QString name;
        QString title;
        QString description;
    };

    struct Tab {
        QString id;
        QString dashboardId;
        QString title;
        QString icon;
    };

    struct Module {
        QString id;
        QString tabId;
        QList<QString> streamList;
        QString widgetId;
        QString pageId;
        QString name;
        QString title;
        QString status;
        QString icon;
    };

    struct Stream {
        QString id;
        QString title;
        QString content;
        QString link;
        QString query;
        QString icon;
        QString type;
        int unread;
        int read;
        int saved;
        int slow;
        int newestItemAddedAt;
        int updateAt;
        int lastUpdate;
    };

    struct Entry {
        QString id;
        QString streamId;
        QString title;
        QString author;
        QString link;
        QString content;
        QString image;
        QString feedId;
        QString feedIcon;
        QString feedTitle;
        QString annotations;
        int fresh;
        int freshOR;
        int read;
        int saved;
        int liked;
        int cached;
        int broadcast;
        int publishedAt;
        int createdAt;
        int crawlTime;
        int timestamp;
    };

    struct CacheItem {
        QString id;
        QString origUrl;
        QString finalUrl;
        QString redirectUrl;
        QString baseUrl;
        QString type;
        QString contentType;
        QString entryId;
        QString streamId;
        int date;
        int flag;
    };

    enum ActionsTypes {
        SetRead = 11,
        UnSetRead = 10,
        SetSaved = 21,
        UnSetSaved = 20,
        SetStreamReadAll = 30,
        UnSetStreamReadAll= 31,
        SetTabReadAll = 40,
        UnSetTabReadAll= 41,
        SetAllRead = 51,
        UnSetAllRead = 50,
        SetSlowRead = 61,
        UnSetSlowRead = 60,
        SetBroadcast = 71,
        UnSetBroadcast = 70,
        SetListRead = 81,
        UnSetListRead = 80,
        SetListSaved = 91,
        UnSetListSaved = 90,
        SetLiked = 101,
        UnSetLiked = 100
    };

    struct Action {
        ActionsTypes type;
        QString id1;
        QString id2;
        QString id3;
        QString text;
        int date1;
        int date2;
        int date3;
    };

    explicit DatabaseManager(QObject *parent = 0);

    Q_INVOKABLE void init();
    Q_INVOKABLE void newInit();

    void cleanDashboards();
    void cleanTabs();
    void cleanModules();
    void cleanStreams();
    void cleanEntries();
    void cleanCache();

    void writeDashboard(const Dashboard &item);
    void writeTab(const Tab &item);
    void writeModule(const Module &item);
    void writeStreamModuleTab(const StreamModuleTab &item);
    void writeStream(const Stream &item);
    void writeEntry(const Entry &item);
    void writeCache(const CacheItem &item);
    void writeAction(const Action &item);
    void updateActionByIdAndType(const QString &oldId1, ActionsTypes oldType, const QString &newId1, const QString &newId2, const QString &newId3, ActionsTypes newType);

    void updateEntriesReadFlagByStream(const QString &id, int flag);
    void updateEntriesReadFlagByDashboard(const QString &id, int flag);
    void updateEntriesSlowReadFlagByDashboard(const QString &id, int flag);
    void updateEntriesReadFlagByTab(const QString &id, int flag);
    void updateEntriesReadFlagByEntry(const QString &id, int flag);
    void updateEntriesSavedFlagByEntry(const QString &id, int flag);
    void updateEntriesCachedFlagByEntry(const QString &id, int cacheDate, int flag);
    void updateEntriesBroadcastFlagByEntry(const QString &id, int flag, const QString &annotations);
    void updateEntriesLikedFlagByEntry(const QString &id, int flag);
    void updateEntriesFreshFlag(int flag);
    void updateEntriesFlag(int flag);
    void updateEntriesSavedFlagByFlagAndDashboard(const QString &id, int flagOld, int flagNew);

    void updateStreamSlowFlagById(const QString &id, int flag);

    bool isDashboardExists();
    bool isCacheExists(const QString &id);
    bool isCacheExistsByFinalUrl(const QString &id);
    bool isCacheExistsByEntryId(const QString &id);

    Dashboard readDashboard(const QString &id);
    QList<Action> readActions();
    QList<Dashboard> readDashboards();
    QList<Tab> readTabsByDashboard(const QString &id);
    QList<Stream> readStreamsByTab(const QString &id);
    QList<QString> readStreamIdsByTab(const QString &id);
    QList<Stream> readStreamsByDashboard(const QString &id);
    QList<QString> readTabIdsByDashboard(const QString &id);
    QList<QString> readStreamIds();
    QString readStreamIdByEntry(const QString &id);
    QList<QString> readModuleIdByStream(const QString &id);
    QMap<QString,QString> readStreamIdsTabIds();
    QList<StreamModuleTab> readStreamModuleTabList();
    QList<StreamModuleTab> readStreamModuleTabListByTab(const QString &id);
    QList<StreamModuleTab> readStreamModuleTabListByDashboard(const QString &id);
    QList<StreamModuleTab> readSlowStreamModuleTabListByDashboard(const QString &id);
    QList<StreamModuleTab> readStreamModuleTabListWithoutDate();

    QString readEntryImageById(const QString &id);

    QString readLatestEntryIdByDashboard(const QString &id);
    QString readLatestEntryIdByTab(const QString &id);
    QString readLatestEntryIdByStream(const QString &id);

    QList<Entry> readEntriesByDashboard(const QString &id, int offset, int limit, bool ascOrder = false);
    QList<Entry> readEntriesUnreadByDashboard(const QString &id, int offset, int limit, bool ascOrder = false);
    QList<Entry> readEntriesSlowUnreadByDashboard(const QString &id, int offset, int limit, bool ascOrder = false);
    QList<Entry> readEntriesSavedByDashboard(const QString &id, int offset, int limit, bool ascOrder = false);
    //QList<Entry> readEntriesSaved(int offset, int limit, bool ascOrder = false);
    QList<Entry> readEntriesSlowByDashboard(const QString &id, int offset, int limit, bool ascOrder = false);
    QList<Entry> readEntriesLikedByDashboard(const QString &id, int offset, int limit, bool ascOrder = false);
    QList<Entry> readEntriesBroadcastByDashboard(const QString &id, int offset, int limit, bool ascOrder = false);
    QList<Entry> readEntriesByStream(const QString &id, int offset, int limit, bool ascOrder = false);
    QList<Entry> readEntriesUnreadByStream(const QString &id, int offset, int limit, bool ascOrder = false);
    QList<Entry> readEntriesByTab(const QString &id, int offset, int limit, bool ascOrder = false);
    QList<Entry> readEntriesUnreadByTab(const QString &id, int offset, int limit, bool ascOrder = false);
    QList<Entry> readEntriesCachedOlderThan(int cacheDate, int limit);
    QList<QString> readCacheFinalUrlOlderThan(int cacheDate, int limit);
    QList<QString> readCacheIdsOlderThan(int cacheDate, int limit);

    CacheItem readCacheByOrigUrl(const QString &id);
    CacheItem readCacheByEntry(const QString &id);
    CacheItem readCacheByFinalUrl(const QString &id);
    CacheItem readCacheByCache(const QString &id);
    QList<QString> readCacheFinalUrlsByStream(const QString &id, int limit);
    QMap<QString,QString> readNotCachedEntries();

    int readLastUpdateByTab(const QString &id);
    int readLastUpdateByDashboard(const QString &id);
    int readLastUpdateByStream(const QString &id);

    int readLastPublishedAtByTab(const QString &id);
    int readLastTimestampByTab(const QString &id);
    int readLastCrawlTimeByTab(const QString &id);
    int readLastLastUpdateByTab(const QString &id);

    int readLastPublishedAtByDashboard(const QString &id);
    int readLastTimestampByDashboard(const QString &id);
    int readLastCrawlTimeByDashboard(const QString &id);
    int readLastLastUpdateByDashboard(const QString &id);

    int readLastPublishedAtByStream(const QString &id);
    int readLastTimestampByStream(const QString &id);
    int readLastCrawlTimeByStream(const QString &id);
    int readLastLastUpdateByStream(const QString &id);

    int readLastPublishedAtSlowByDashboard(const QString &id);
    int readLastTimestampSlowByDashboard(const QString &id);
    int readLastCrawlTimeSlowByDashboard(const QString &id);
    int readLastLastUpdateSlowByDashboard(const QString &id);

    void removeTabById(const QString &id);
    void removeStreamsByStream(const QString &id);
    //void removeEntriesOlderThan(int cacheDate, int limit);
    //void removeEntriesOlderThanByCrawlTime(int cacheDate);
    void removeEntriesByStream(const QString &id, int limit);
    void removeEntriesByFlag(int value);
    void removeActionsById(const QString &id);
    void removeActionsByIdAndType(const QString &id, ActionsTypes type);
    //void removeEntriesBySavedFlag(int flag);
    void removeCacheItems();

    int countEntries();
    int countStreams();
    int countTabs();
    int countEntriesByStream(const QString &id);
    int countEntriesNewerThanByStream(const QString &id, const QDateTime &date);
    int countEntriesUnreadByStream(const QString &id);
    int countEntriesUnreadByTab(const QString &id);
    int countEntriesReadByStream(const QString &id);
    int countEntriesReadByTab(const QString &id);
    int countEntriesFreshByStream(const QString &id);
    int countEntriesFreshByTab(const QString &id);
    int countEntriesReadByDashboard(const QString &id);
    int countEntriesUnreadByDashboard(const QString &id);
    int countEntriesSlowReadByDashboard(const QString &id);
    int countEntriesSlowUnreadByDashboard(const QString &id);
    int countEntriesNotCached();

signals:
    /*
    500 - DB can not be opened
    501 - Creation of new empty DB failed
    502 - Check DB parameters failed
    511 - The database disk image is malformed
    */
    void error(int code);

    void empty();
    void notEmpty();

private:
    static const QString version;
    QSqlDatabase db;
    QString dbFilePath;

    void checkError(const QSqlError &error);

    bool openDB();
    bool createDB();
    bool alterDB_19to22();
    bool alterDB_20to22();
    bool alterDB_21to22();
    bool deleteDB();

    bool createStructure();
    bool createDashboardsStructure();
    bool createTabsStructure();
    bool createModulesStructure();
    bool createStreamsStructure();
    bool createEntriesStructure();
    bool createCacheStructure();
    bool createActionsStructure();
    bool checkParameters();
    bool isTableExists(const QString &name);
    void decodeBase64(const QVariant &source, QString &result);
};

Q_DECLARE_METATYPE(DatabaseManager::CacheItem)

#endif // DATABASEMANAGER_H

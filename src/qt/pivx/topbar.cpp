// Copyright (c) 2019-2020 The PIVX developers
// Copyright (c) 2021 The DECENOMY Core Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "qt/pivx/topbar.h"
#include "askpassphrasedialog.h"
#include "qt/pivx/forms/ui_topbar.h"
#include "qt/pivx/loadingdialog.h"
#include "qt/pivx/lockunlock.h"
#include "qt/pivx/qtutils.h"
#include "qt/pivx/receivedialog.h"

#include "addresstablemodel.h"
#include "bitcoinunits.h"
#include "clientmodel.h"
#include "guiinterface.h"
#include "optionsmodel.h"
#include "qt/guiconstants.h"
#include "qt/guiutil.h"
#include "qt/platformstyle.h"
#include "walletmodel.h"

#include "masternode-sync.h"
#include "masternodeman.h"
#include "wallet/wallet.h"

#include <QPixmap>

#define REQUEST_UPGRADE_WALLET 1

TopBar::TopBar(PIVXGUI* _mainWindow, QWidget* parent) : PWidget(_mainWindow, parent),
                                                        ui(new Ui::TopBar)
{
    ui->setupUi(this);

    // Set parent stylesheet
    this->setStyleSheet(_mainWindow->styleSheet());
    /* Containers */
    ui->containerTop->setContentsMargins(10, 4, 10, 10);
    ui->containerTop->setProperty("cssClass", "container-top");

    setCssProperty({ui->labelTitle1, ui->labelTitle3, ui->labelTitle4, ui->labelTitle5,
                       ui->labelTitle6, ui->labelMasternodesTitle, ui->labelTitle8,
                       ui->labelNextMasternodesTitle, ui->labelTitle9},
        "text-title-topbar");

    // Amount information top
    ui->widgetTopAmount->setVisible(false);
    ui->widgetAmount->setVisible(true);
    setCssProperty({ui->labelAmountTopPiv}, "amount-small-topbar");
    setCssProperty({ui->labelAmountPiv}, "amount-topbar");
    setCssProperty({ui->labelPendingPiv, ui->labelImmaturePiv, ui->labelAvailablePiv,
                       ui->labelLockedPiv, ui->labelMasternodeCount, ui->labelCollateralPiv,
                       ui->labelNextCollateralBlocks, ui->labelNextCollateralValue},
        "amount-small-topbar");

    // Next masternode collateral
    ui->widgetNextCollateral->setVisible(false);

    // Progress Sync
    progressBar = new QProgressBar(ui->layoutSync);
    progressBar->setRange(1, 10);
    progressBar->setValue(4);
    progressBar->setTextVisible(false);
    progressBar->setMaximumHeight(2);
    progressBar->setMaximumWidth(36);
    setCssProperty(progressBar, "progress-sync");
    progressBar->show();
    progressBar->raise();
    progressBar->move(0, 34);

    ui->pushButtonFAQ->setButtonClassStyle("cssClass", "btn-check-faq");
    ui->pushButtonFAQ->setButtonText(tr("FAQ"));

    ui->pushButtonHDUpgrade->setButtonClassStyle("cssClass", "btn-check-hd-upgrade");
    ui->pushButtonHDUpgrade->setButtonText(tr("Upgrade to HD Wallet"));
    ui->pushButtonHDUpgrade->setNoIconText("HD");

    ui->pushButtonConnection->setButtonClassStyle("cssClass", "btn-check-connect-inactive");
    ui->pushButtonConnection->setButtonText(tr("No Connection"));

    ui->pushButtonTor->setButtonClassStyle("cssClass", "btn-check-tor-inactive");
    ui->pushButtonTor->setButtonText(tr("Tor Disabled"));
    ui->pushButtonTor->setChecked(false);
    ui->pushButtonTor->setVisible(false);

    ui->pushButtonStack->setButtonClassStyle("cssClass", "btn-check-stack-inactive");
    ui->pushButtonStack->setButtonText(tr("Staking Disabled"));

    ui->pushButtonConf->setButtonClassStyle("cssClass", "btn-check-conf");
    ui->pushButtonConf->setButtonText("trittium.conf");
    ui->pushButtonConf->setChecked(false);

    ui->pushButtonMasternodes->setButtonClassStyle("cssClass", "btn-check-masternodes");
    ui->pushButtonMasternodes->setButtonText("masternode.conf");
    ui->pushButtonMasternodes->setChecked(false);

    ui->pushButtonConsole->setButtonClassStyle("cssClass", "btn-check-console");
    ui->pushButtonConsole->setButtonText("Debug Console");
    ui->pushButtonConsole->setChecked(false);

    ui->pushButtonSync->setButtonClassStyle("cssClass", "btn-check-sync");
    ui->pushButtonSync->setButtonText(tr(" %54 Synchronizing.."));

    ui->pushButtonLock->setButtonClassStyle("cssClass", "btn-check-lock");

    if (isLightTheme()) {
        ui->pushButtonTheme->setButtonClassStyle("cssClass", "btn-check-theme-light");
        ui->pushButtonTheme->setButtonText(tr("Light Theme"));
    } else {
        ui->pushButtonTheme->setButtonClassStyle("cssClass", "btn-check-theme-dark");
        ui->pushButtonTheme->setButtonText(tr("Dark Theme"));
    }

    setCssProperty(ui->qrContainer, "container-qr");
    setCssProperty(ui->pushButtonQR, "btn-qr");

    // QR image
    QPixmap pixmap("://img-qr-test");
    ui->btnQr->setIcon(
        QIcon(pixmap.scaled(
            70,
            70,
            Qt::KeepAspectRatio)));

    ui->pushButtonLock->setButtonText(tr("Wallet Locked "));
    ui->pushButtonLock->setButtonClassStyle("cssClass", "btn-check-status-lock");


    connect(ui->pushButtonQR, &QPushButton::clicked, this, &TopBar::onBtnReceiveClicked);
    connect(ui->btnQr, &QPushButton::clicked, this, &TopBar::onBtnReceiveClicked);
    connect(ui->pushButtonLock, &ExpandableButton::Mouse_Pressed, this, &TopBar::onBtnLockClicked);
    connect(ui->pushButtonTheme, &ExpandableButton::Mouse_Pressed, this, &TopBar::onThemeClicked);
    connect(ui->pushButtonFAQ, &ExpandableButton::Mouse_Pressed, [this]() { window->openFAQ(); });
    connect(ui->pushButtonConf, &ExpandableButton::Mouse_Pressed, this, &TopBar::onBtnConfClicked);
    connect(ui->pushButtonMasternodes, &ExpandableButton::Mouse_Pressed, this, &TopBar::onBtnMasternodesClicked);
    connect(ui->pushButtonSync, &ExpandableButton::Mouse_HoverLeave, this, &TopBar::refreshProgressBarSize);
    connect(ui->pushButtonSync, &ExpandableButton::Mouse_Hover, this, &TopBar::refreshProgressBarSize);
    connect(ui->pushButtonSync, &ExpandableButton::Mouse_Pressed, [this]() { window->goToSettingsInfo(); });
    connect(ui->pushButtonConsole, &ExpandableButton::Mouse_Pressed, [this]() { window->goToDebugConsole(); });
    connect(ui->pushButtonConnection, &ExpandableButton::Mouse_Pressed, [this]() { window->showPeers(); });

    refreshStatus();
}

void TopBar::onThemeClicked()
{
    // Store theme
    bool lightTheme = !isLightTheme();

    setTheme(lightTheme);

    if (lightTheme) {
        ui->pushButtonTheme->setButtonClassStyle("cssClass", "btn-check-theme-light", true);
        ui->pushButtonTheme->setButtonText(tr("Light Theme"));
    } else {
        ui->pushButtonTheme->setButtonClassStyle("cssClass", "btn-check-theme-dark", true);
        ui->pushButtonTheme->setButtonText(tr("Dark Theme"));
    }
    updateStyle(ui->pushButtonTheme);

    Q_EMIT themeChanged(lightTheme);
}

void TopBar::onBtnLockClicked()
{
    if (walletModel) {
        if (walletModel->getEncryptionStatus() == WalletModel::Unencrypted) {
            encryptWallet();
        } else {
            if (!lockUnlockWidget) {
                lockUnlockWidget = new LockUnlock(window);
                lockUnlockWidget->setStyleSheet("margin:0px; padding:0px;");
                connect(lockUnlockWidget, &LockUnlock::Mouse_Leave, this, &TopBar::lockDropdownMouseLeave);
                connect(ui->pushButtonLock, &ExpandableButton::Mouse_HoverLeave, [this]() {
                    QMetaObject::invokeMethod(this, "lockDropdownMouseLeave", Qt::QueuedConnection);
                });
                connect(lockUnlockWidget, &LockUnlock::lockClicked, this, &TopBar::lockDropdownClicked);
            }

            lockUnlockWidget->updateStatus(walletModel->getEncryptionStatus());
            if (ui->pushButtonLock->width() <= 40) {
                ui->pushButtonLock->setExpanded();
            }
            // Keep it open
            ui->pushButtonLock->setKeepExpanded(true);
            QMetaObject::invokeMethod(this, "openLockUnlock", Qt::QueuedConnection);
        }
    }
}

void TopBar::openLockUnlock()
{
    lockUnlockWidget->setFixedWidth(ui->pushButtonLock->width());
    lockUnlockWidget->adjustSize();

    lockUnlockWidget->move(
        ui->pushButtonLock->pos().rx() + window->getNavWidth() + 10,
        ui->pushButtonLock->y() + 36);

    lockUnlockWidget->raise();
    lockUnlockWidget->activateWindow();
    lockUnlockWidget->show();
}

void TopBar::openPassPhraseDialog(AskPassphraseDialog::Mode mode, AskPassphraseDialog::Context ctx)
{
    if (!walletModel)
        return;

    showHideOp(true);
    AskPassphraseDialog* dlg = new AskPassphraseDialog(mode, window, walletModel, ctx);
    dlg->adjustSize();
    openDialogWithOpaqueBackgroundY(dlg, window);

    refreshStatus();
    dlg->deleteLater();
}

void TopBar::encryptWallet()
{
    return openPassPhraseDialog(AskPassphraseDialog::Mode::Encrypt, AskPassphraseDialog::Context::Encrypt);
}

void TopBar::unlockWallet()
{
    if (!walletModel)
        return;
    // Unlock wallet when requested by wallet model (if unlocked or unlocked for staking only)
    if (walletModel->isWalletLocked(false))
        return openPassPhraseDialog(AskPassphraseDialog::Mode::Unlock, AskPassphraseDialog::Context::Unlock_Full);
}

static bool isExecuting = false;

void TopBar::lockDropdownClicked(const StateClicked& state)
{
    lockUnlockWidget->close();
    if (walletModel && !isExecuting) {
        isExecuting = true;

        switch (lockUnlockWidget->lock) {
        case 0: {
            if (walletModel->getEncryptionStatus() == WalletModel::Locked)
                break;
            walletModel->setWalletLocked(true);
            ui->pushButtonLock->setButtonText(tr("Wallet Locked"));
            ui->pushButtonLock->setButtonClassStyle("cssClass", "btn-check-status-lock", true);
            // Directly update the staking status icon when the wallet is manually locked here
            // so the feedback is instant (no need to wait for the polling timeout)
            setStakingStatusActive(false);
            break;
        }
        case 1: {
            if (walletModel->getEncryptionStatus() == WalletModel::Unlocked)
                break;
            showHideOp(true);
            AskPassphraseDialog* dlg = new AskPassphraseDialog(AskPassphraseDialog::Mode::Unlock, window, walletModel,
                AskPassphraseDialog::Context::ToggleLock);
            dlg->adjustSize();
            openDialogWithOpaqueBackgroundY(dlg, window);
            if (walletModel->getEncryptionStatus() == WalletModel::Unlocked) {
                ui->pushButtonLock->setButtonText(tr("Wallet Unlocked"));
                ui->pushButtonLock->setButtonClassStyle("cssClass", "btn-check-status-unlock", true);
            }
            dlg->deleteLater();
            break;
        }
        case 2: {
            WalletModel::EncryptionStatus status = walletModel->getEncryptionStatus();
            if (status == WalletModel::UnlockedForStaking)
                break;

            if (status == WalletModel::Unlocked) {
                walletModel->lockForStakingOnly();
            } else {
                showHideOp(true);
                AskPassphraseDialog* dlg = new AskPassphraseDialog(AskPassphraseDialog::Mode::UnlockAnonymize,
                    window, walletModel,
                    AskPassphraseDialog::Context::ToggleLock);
                dlg->adjustSize();
                openDialogWithOpaqueBackgroundY(dlg, window);
                dlg->deleteLater();
            }
            if (walletModel->getEncryptionStatus() == WalletModel::UnlockedForStaking) {
                ui->pushButtonLock->setButtonText(tr("Wallet Unlocked for staking"));
                ui->pushButtonLock->setButtonClassStyle("cssClass", "btn-check-status-staking", true);
            }
            break;
        }
        }

        ui->pushButtonLock->setKeepExpanded(false);
        ui->pushButtonLock->setSmall();
        ui->pushButtonLock->update();

        isExecuting = false;
    }
}

void TopBar::lockDropdownMouseLeave()
{
    if (lockUnlockWidget->isVisible() && !lockUnlockWidget->isHovered()) {
        lockUnlockWidget->hide();
        ui->pushButtonLock->setKeepExpanded(false);
        ui->pushButtonLock->setSmall();
        ui->pushButtonLock->update();
    }
}

void TopBar::onBtnReceiveClicked()
{
    if (walletModel) {
        QString addressStr = walletModel->getAddressTableModel()->getAddressToShow();
        if (addressStr.isNull()) {
            inform(tr("Error generating address"));
            return;
        }
        showHideOp(true);
        ReceiveDialog* receiveDialog = new ReceiveDialog(window);
        receiveDialog->updateQr(addressStr);
        if (openDialogWithOpaqueBackground(receiveDialog, window)) {
            inform(tr("Address Copied"));
        }
        receiveDialog->deleteLater();
    }
}

void TopBar::showTop()
{
    if (ui->bottom_container->isVisible()) {
        ui->bottom_container->setVisible(false);
        ui->widgetTopAmount->setVisible(true);
        ui->widgetAmount->setVisible(false);
        this->setFixedHeight(75);
    }
}

void TopBar::showBottom()
{
    ui->widgetTopAmount->setVisible(false);
    ui->widgetAmount->setVisible(true);
    ui->bottom_container->setVisible(true);
    this->setFixedHeight(200);
    this->adjustSize();
}

void TopBar::onBtnConfClicked()
{
    ui->pushButtonConf->setChecked(false);

    if (!GUIUtil::openConfigfile())
        inform(tr("Unable to open trittium.conf with default application"));
}

void TopBar::onBtnMasternodesClicked()
{
    ui->pushButtonMasternodes->setChecked(false);

    if (!GUIUtil::openMNConfigfile())
        inform(tr("Unable to open masternode.conf with default application"));
}

TopBar::~TopBar()
{
    if (timerStakingIcon) {
        timerStakingIcon->stop();
    }
    delete ui;
}

void TopBar::loadClientModel()
{
    if (clientModel) {
        // Keep up to date with client
        setNumConnections(clientModel->getNumConnections());
        connect(clientModel, &ClientModel::numConnectionsChanged, this, &TopBar::setNumConnections);

        setNumBlocks(clientModel->getNumBlocks());
        connect(clientModel, &ClientModel::numBlocksChanged, this, &TopBar::setNumBlocks);

        timerStakingIcon = new QTimer(ui->pushButtonStack);
        connect(timerStakingIcon, &QTimer::timeout, this, &TopBar::updateStakingStatus);
        timerStakingIcon->start(50000);
        updateStakingStatus();
    }
}

void TopBar::setStakingStatusActive(bool fActive)
{
    if (ui->pushButtonStack->isChecked() != fActive) {
        ui->pushButtonStack->setButtonText(fActive ? tr("Staking active") : tr("Staking not active"));
        ui->pushButtonStack->setChecked(fActive);
        ui->pushButtonStack->setButtonClassStyle("cssClass", (fActive ? "btn-check-stack" : "btn-check-stack-inactive"), true);
    }
}
void TopBar::updateStakingStatus()
{
    setStakingStatusActive(walletModel &&
                           !walletModel->isWalletLocked() &&
                           walletModel->isStakingStatusActive());

    // Taking advantage of this timer to update Tor status if needed.
    updateTorIcon();
}

void TopBar::setNumConnections(int count)
{
    if (count > 0) {
        if (!ui->pushButtonConnection->isChecked()) {
            ui->pushButtonConnection->setChecked(true);
            ui->pushButtonConnection->setButtonClassStyle("cssClass", "btn-check-connect", true);
        }
    } else {
        if (ui->pushButtonConnection->isChecked()) {
            ui->pushButtonConnection->setChecked(false);
            ui->pushButtonConnection->setButtonClassStyle("cssClass", "btn-check-connect-inactive", true);
        }
    }

    ui->pushButtonConnection->setButtonText(tr("%n active connection(s)", "", count));
}

void TopBar::setNumBlocks(int count)
{
    if (!clientModel)
        return;

    // Acquire current block source
    enum BlockSource blockSource = clientModel->getBlockSource();
    std::string text = "";
    switch (blockSource) {
    case BLOCK_SOURCE_NETWORK:
        text = "Synchronizing..";
        break;
    case BLOCK_SOURCE_DISK:
        text = "Importing blocks from disk..";
        break;
    case BLOCK_SOURCE_REINDEX:
        text = "Reindexing blocks on disk..";
        break;
    case BLOCK_SOURCE_NONE:
        // Case: not Importing, not Reindexing and no network connection
        text = "No block source available..";
        ui->pushButtonSync->setChecked(false);
        break;
    }

    bool needState = true;
    if (masternodeSync.IsBlockchainSynced()) {
        // chain synced
        Q_EMIT walletSynced(true);
        if (masternodeSync.IsSynced()) {
            // Node synced
            ui->pushButtonSync->setButtonText(tr("Synchronized - Block: %1").arg(QString::number(count)));
            progressBar->setRange(0, 100);
            progressBar->setValue(100);
            refreshStatus();
            return;
        } else {
            // TODO: Show out of sync warning
            int nAttempt = masternodeSync.RequestedMasternodeAttempt < MASTERNODE_SYNC_THRESHOLD ?
                               masternodeSync.RequestedMasternodeAttempt + 1 :
                               MASTERNODE_SYNC_THRESHOLD;
            int progress = nAttempt + (masternodeSync.RequestedMasternodeAssets - 1) * MASTERNODE_SYNC_THRESHOLD;
            if (progress >= 0) {
                // todo: MN progress..
                text = strprintf("%s - Block: %d", masternodeSync.GetSyncStatus(), count);
                //progressBar->setMaximum(4 * MASTERNODE_SYNC_THRESHOLD);
                //progressBar->setValue(progress);
                needState = false;
            }
        }
    } else {
        Q_EMIT walletSynced(false);
    }

    if (needState) {
        // Represent time from last generated block in human readable text
        QDateTime lastBlockDate = clientModel->getLastBlockDate();
        QDateTime currentDate = QDateTime::currentDateTime();
        int secs = lastBlockDate.secsTo(currentDate);

        QString timeBehindText;
        const int HOUR_IN_SECONDS = 60 * 60;
        const int DAY_IN_SECONDS = 24 * 60 * 60;
        const int WEEK_IN_SECONDS = 7 * 24 * 60 * 60;
        const int YEAR_IN_SECONDS = 31556952; // Average length of year in Gregorian calendar
        if (secs < 2 * DAY_IN_SECONDS) {
            timeBehindText = tr("%n hour(s)", "", secs / HOUR_IN_SECONDS);
        } else if (secs < 2 * WEEK_IN_SECONDS) {
            timeBehindText = tr("%n day(s)", "", secs / DAY_IN_SECONDS);
        } else if (secs < YEAR_IN_SECONDS) {
            timeBehindText = tr("%n week(s)", "", secs / WEEK_IN_SECONDS);
        } else {
            int years = secs / YEAR_IN_SECONDS;
            int remainder = secs % YEAR_IN_SECONDS;
            timeBehindText = tr("%1 and %2").arg(tr("%n year(s)", "", years)).arg(tr("%n week(s)", "", remainder / WEEK_IN_SECONDS));
        }
        QString timeBehind(" behind. Scanning block ");
        QString str = timeBehindText + timeBehind + QString::number(count);
        text = str.toStdString();

        progressBar->setMaximum(1000000000);
        progressBar->setValue(clientModel->getVerificationProgress() * 1000000000.0 + 0.5);
    }

    if (text.empty()) {
        text = "No block source available..";
    }

    ui->pushButtonSync->setButtonText(tr(text.data()));
}

void TopBar::showUpgradeDialog()
{
    QString title = tr("Wallet Upgrade");
    if (ask(title,
            tr("Upgrading to HD wallet will improve\nthe wallet's reliability and security.\n\n\n"
               "NOTE: after the upgrade, a new\nbackup will be created.\n"))) {
        std::unique_ptr<WalletModel::UnlockContext> pctx = MakeUnique<WalletModel::UnlockContext>(walletModel->requestUnlock());
        if (!pctx->isValid()) {
            warn(tr("Upgrade Wallet"), tr("Wallet unlock cancelled"));
            return;
        }
        // Action performed on a separate thread, it's locking cs_main and cs_wallet.
        LoadingDialog* dialog = new LoadingDialog(window);
        dialog->execute(this, REQUEST_UPGRADE_WALLET, std::move(pctx));
        openDialogWithOpaqueBackgroundFullScreen(dialog, window);
    }
}

void TopBar::loadWalletModel()
{
    // Upgrade wallet.
    if (walletModel->isHDEnabled()) {
        ui->pushButtonHDUpgrade->setVisible(false);
    } else {
        connect(ui->pushButtonHDUpgrade, &ExpandableButton::Mouse_Pressed, this, &TopBar::showUpgradeDialog);

        // Upgrade wallet timer, only once. launched 4 seconds after the wallet started.
        QTimer::singleShot(4000, [this]() {
            showUpgradeDialog();
        });
    }

    connect(walletModel, &WalletModel::balanceChanged, this, &TopBar::updateBalances);
    connect(walletModel->getOptionsModel(), &OptionsModel::displayUnitChanged, this, &TopBar::updateDisplayUnit);
    connect(walletModel, &WalletModel::encryptionStatusChanged, this, &TopBar::refreshStatus);
    // Ask for passphrase if needed
    connect(walletModel, &WalletModel::requireUnlock, this, &TopBar::unlockWallet);
    // update the display unit, to not use the default ("TRTT")
    updateDisplayUnit();

    refreshStatus();

    isInitializing = false;
}

void TopBar::updateTorIcon()
{
    std::string ip_port;
    bool torEnabled = clientModel->getTorInfo(ip_port);

    if (torEnabled) {
        if (!ui->pushButtonTor->isChecked()) {
            ui->pushButtonTor->setChecked(true);
            ui->pushButtonTor->setButtonClassStyle("cssClass", "btn-check-tor", true);
        }
        QString ip_port_q = QString::fromStdString(ip_port);
        ui->pushButtonTor->setButtonText(tr("Tor Active: %1").arg(ip_port_q));
    } else {
        if (ui->pushButtonTor->isChecked()) {
            ui->pushButtonTor->setChecked(false);
            ui->pushButtonTor->setButtonClassStyle("cssClass", "btn-check-tor-inactive", true);
            ui->pushButtonTor->setButtonText(tr("Tor Disabled"));
        }
    }
}

void TopBar::refreshMasternodeStatus()
{
    // Masternodes
    int nMNCount = 0;
    int nMNActive = 0;
    bool isSynced = masternodeSync.IsSynced();

    for (auto mne : masternodeConfig.getEntries()) {
        nMNCount++;

        if (isSynced) {
            int nIndex;
            if (!mne.castOutputIndex(nIndex))
                continue;

            uint256 txHash(mne.getTxHash());
            CTxIn txIn(txHash, uint32_t(nIndex));
            auto pmn = mnodeman.Find(txIn);

            if (!pmn) continue;

            int activeState = pmn->activeState;

            if (activeState == CMasternode::MASTERNODE_PRE_ENABLED || activeState == CMasternode::MASTERNODE_ENABLED) {
                nMNActive++;
            }
        }
    }

    ui->labelMasternodeCount->setText(tr("%1/%2").arg(isSynced ? std::to_string(nMNActive).c_str() : "--").arg(nMNCount));
    ui->labelMasternodesTitle->setText(tr("Masternodes%1").arg(isSynced ? "" : " (Syncing)"));

    if(chainActive.Tip()) {
        auto p = CMasternode::GetNextMasternodeCollateral(chainActive.Tip()->nHeight);

        ui->widgetNextCollateral->setVisible(p.first > 0);
        if(p.first > 0) {
            ui->labelNextCollateralValue->setText(GUIUtil::formatBalance(p.second, nDisplayUnit));
            ui->labelNextCollateralBlocks->setText(tr("%1 Blocks").arg(p.first));
        }
    }
}

void TopBar::refreshStatus()
{
    refreshMasternodeStatus();

    // Check lock status
    if (!this->walletModel)
        return;

    WalletModel::EncryptionStatus encStatus = walletModel->getEncryptionStatus();

    switch (encStatus) {
    case WalletModel::EncryptionStatus::Unencrypted:
        ui->pushButtonLock->setButtonText(tr("Wallet Unencrypted"));
        ui->pushButtonLock->setButtonClassStyle("cssClass", "btn-check-status-unlock", true);
        break;
    case WalletModel::EncryptionStatus::Locked:
        ui->pushButtonLock->setButtonText(tr("Wallet Locked"));
        ui->pushButtonLock->setButtonClassStyle("cssClass", "btn-check-status-lock", true);
        break;
    case WalletModel::EncryptionStatus::UnlockedForStaking:
        ui->pushButtonLock->setButtonText(tr("Wallet Unlocked for staking"));
        ui->pushButtonLock->setButtonClassStyle("cssClass", "btn-check-status-staking", true);
        break;
    case WalletModel::EncryptionStatus::Unlocked:
        ui->pushButtonLock->setButtonText(tr("Wallet Unlocked"));
        ui->pushButtonLock->setButtonClassStyle("cssClass", "btn-check-status-unlock", true);
        break;
    }
    updateStyle(ui->pushButtonLock);

    // Collateral
    ui->labelCollateralPiv->setText(GUIUtil::formatBalance(CMasternode::GetMasternodeNodeCollateral(chainActive.Tip()->nHeight), nDisplayUnit));
}

void TopBar::updateDisplayUnit()
{
    if (walletModel && walletModel->getOptionsModel()) {
        int displayUnitPrev = nDisplayUnit;
        nDisplayUnit = walletModel->getOptionsModel()->getDisplayUnit();
        if (displayUnitPrev != nDisplayUnit)
            updateBalances(walletModel->GetWalletBalances());
    }
}

void TopBar::updateBalances(const interfaces::WalletBalances& newBalance)
{
    // Locked balance. //TODO move this to the signal properly in the future..
    CAmount nLockedBalance = 0;
    if (walletModel) {
        nLockedBalance = walletModel->getLockedBalance();
    }

    CAmount nAvailableBalance = newBalance.balance - nLockedBalance;

    // TRTT
    // Top
    ui->labelAmountTopPiv->setText(GUIUtil::formatBalance(nAvailableBalance, nDisplayUnit));
    // Expanded
    ui->labelAmountPiv->setText(GUIUtil::formatBalance(newBalance.balance + newBalance.immature_balance, nDisplayUnit));
    ui->labelAvailablePiv->setText(GUIUtil::formatBalance(nAvailableBalance, nDisplayUnit));
    ui->labelPendingPiv->setText(GUIUtil::formatBalance(newBalance.unconfirmed_balance, nDisplayUnit));
    ui->labelImmaturePiv->setText(GUIUtil::formatBalance(newBalance.immature_balance, nDisplayUnit));
    ui->labelLockedPiv->setText(GUIUtil::formatBalance(nLockedBalance, nDisplayUnit));

    refreshMasternodeStatus();

    // Collateral
    ui->labelCollateralPiv->setText(GUIUtil::formatBalance(CMasternode::GetMasternodeNodeCollateral(chainActive.Tip()->nHeight), nDisplayUnit));
}

void TopBar::resizeEvent(QResizeEvent* event)
{
    if (lockUnlockWidget && lockUnlockWidget->isVisible()) lockDropdownMouseLeave();
    QWidget::resizeEvent(event);
}

void TopBar::refreshProgressBarSize()
{
    QMetaObject::invokeMethod(this, "expandSync", Qt::QueuedConnection);
}

void TopBar::expandSync()
{
    if (progressBar) {
        progressBar->setMaximumWidth(ui->pushButtonSync->maximumWidth());
        progressBar->setFixedWidth(ui->pushButtonSync->width());
        progressBar->setMinimumWidth(ui->pushButtonSync->width() - 2);
    }
}

void TopBar::updateHDState(const bool& upgraded, const QString& upgradeError)
{
    if (upgraded) {
        ui->pushButtonHDUpgrade->setVisible(false);
        if (ask("HD Upgrade Complete", tr("The wallet has been successfully upgraded to HD.") + "\n" +
                                           tr("It is advised to make a backup.") + "\n\n" + tr("Do you wish to backup now?") + "\n\n")) {
            // backup wallet
            QString filename = GUIUtil::getSaveFileName(this,
                tr("Backup Wallet"), QString(),
                tr("Wallet Data (*.dat)"), NULL);
            if (!filename.isEmpty()) {
                inform(walletModel->backupWallet(filename) ? tr("Backup created") : tr("Backup creation failed"));
            } else {
                warn(tr("Backup creation failed"), tr("no file selected"));
            }
        } else {
            inform(tr("Wallet upgraded successfully, but no backup created.") + "\n" +
                   tr("WARNING: remember to make a copy of your wallet file!"));
        }
    } else {
        warn(tr("Upgrade Wallet Error"), upgradeError);
    }
}

void TopBar::run(int type)
{
    if (type == REQUEST_UPGRADE_WALLET) {
        std::string upgradeError;
        bool ret = this->walletModel->upgradeWallet(upgradeError);
        QMetaObject::invokeMethod(this,
            "updateHDState",
            Qt::QueuedConnection,
            Q_ARG(bool, ret),
            Q_ARG(QString, QString::fromStdString(upgradeError)));
    }
}

void TopBar::onError(QString error, int type)
{
    if (type == REQUEST_UPGRADE_WALLET) {
        warn(tr("Upgrade Wallet Error"), error);
    }
}
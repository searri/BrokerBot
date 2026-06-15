// Stocks & Bonds Game Administrator Engine

// 1. Stock Data Definition
const initialStocks = [
    { id: 0, name: "Central City Municipal Bonds", dividend: 50, prices: [100], displayPrices: ["100"], states: ["normal"] },
    { id: 1, name: "Growth Corporation of America", dividend: 10, prices: [100], displayPrices: ["100"], states: ["normal"] },
    { id: 2, name: "Metro Properties, Inc.", dividend: 0, prices: [100], displayPrices: ["100"], states: ["normal"] },
    { id: 3, name: "Pioneer Mutual Fund", dividend: 40, prices: [100], displayPrices: ["100"], states: ["normal"] },
    { id: 4, name: "Shady Brooks Development", dividend: 70, prices: [100], displayPrices: ["100"], states: ["normal"] },
    { id: 5, name: "Stryker Drilling Company", dividend: 0, prices: [100], displayPrices: ["100"], states: ["normal"] },
    { id: 6, name: "Tri-City Transport Company", dividend: 0, prices: [100], displayPrices: ["100"], states: ["normal"] },
    { id: 7, name: "United Auto Company", dividend: 20, prices: [100], displayPrices: ["100"], states: ["normal"] },
    { id: 8, name: "Uranium Enterprises, Inc.", dividend: 60, prices: [100], displayPrices: ["100"], states: ["normal"] },
    { id: 9, name: "Valley Power & Light Company", dividend: 30, prices: [100], displayPrices: ["100"], states: ["normal"] }
];

let stocks = JSON.parse(JSON.stringify(initialStocks));

// 2. Dice Roll Lookup Tables (from C++ Arduino source)
// Map rolls 2-12 (index 0-10) to price changes for stocks 1-9 (excluding Bond 0)
const bullMktData = [
    [-2, -10,  -7,  -9,  -2,  -9,  -7, -16,  -4], // Roll 2
    [26,  16,  25,   8, -14,  21,  14,  -4,  17], // Roll 3
    [18,  23,  11,  12,  46,  18,  -5,  34,  15], // Roll 4
    [23,  28,  -2,  11,  56,  19,  30,  29,  14], // Roll 5
    [20,  15,  15,   7, -20,  15,  13, -10,  12], // Roll 6
    [17,  21,  13,  -2,  37,  23,  23,  19,  14], // Roll 7
    [19,  24,  17,   9,  -5,  26,  13,  -7,  15], // Roll 8
    [11,  18,  14,  11,  67,  15,  22,  18,  13], // Roll 9
    [13,  31,   1,  14, -11,  18,  18, -14,  10], // Roll 10
    [14,  -8,  19,  -1,  -9,  25, -10,  13,  19], // Roll 11
    [24,  24,  23,  20,  51,  27,  38,  33,  18]  // Roll 12
];

const bearMktData = [
    [12,  14,  13,  10,  10,  20,  21,  25,  8],  // Roll 2
    [ 7,  -6,  10, -10,  30,   6, -19,  22, -2],  // Roll 3
    [ 9,  10,   7,  -5, -20,  12,  21,  18,  7],  // Roll 4
    [ 7,   8,   5,  -6, -40,   3,  16, -14,  4],  // Roll 5
    [ 8,   6,   4,  -4,  40,   8,   4, -12,  3],  // Roll 6
    [ 6,   4,   3,   3, -15,   5,   8,  -8,  5],  // Roll 7
    [ 5,   7,  -1,  -3,  45,   6, -10,  10,  4],  // Roll 8
    [-2,   6,  -3,  -8, -20,   7,  10,  14,  6],  // Roll 9
    [11,  11,  -5,  -7,  30,  10, -11, -18, -4],  // Roll 10
    [-5,  13,  -8,   6,  25,   4,  18, -22, -4],  // Roll 11
    [-8, -10, -10, -15, -20, -20, -23, -25, -7]   // Roll 12
];

// 3. Game State Variables
let totalYears = 10;
let currentYear = 1;
let gameActive = false;

// Inputs State for current round
let marketTrend = null; // 'bull' or 'bear'
let priceRoll = null;
let situationCard = null;

// DOM Elements
const setupPanel = document.getElementById("setup-panel");
const gameDashboard = document.getElementById("game-dashboard");
const gameYearsInput = document.getElementById("game-years");
const yearsDisplay = document.getElementById("years-display");
const startGameBtn = document.getElementById("start-game-btn");
const resetGameBtn = document.getElementById("reset-game-btn");

// Control Elements
const currentYearBadge = document.getElementById("current-year-badge");
const trendBullBtn = document.getElementById("trend-bull");
const trendBearBtn = document.getElementById("trend-bear");
const rollMarketBtn = document.getElementById("roll-market-btn");
const marketRollVal = document.getElementById("market-roll-val");

const priceRollInput = document.getElementById("price-roll");
const rollPriceBtn = document.getElementById("roll-price-btn");
const priceDicePreview = document.getElementById("price-dice-preview");

const eventCardInput = document.getElementById("event-card");
const drawCardBtn = document.getElementById("draw-card-btn");
const cardDisplay = document.getElementById("card-display");

const advanceYearBtn = document.getElementById("advance-year-btn");

// Chalkboard & Data Elements
const tableHeaders = document.getElementById("table-headers");
const tableBody = document.getElementById("table-body");
const dividendPayoutList = document.getElementById("dividend-payout-list");
const logConsole = document.getElementById("log-console");

// Modal Elements
const gameOverModal = document.getElementById("game-over-modal");
const finalPricesContainer = document.getElementById("final-prices-container");
const modalCloseBtn = document.getElementById("modal-close-btn");

// Step elements
const step1 = document.getElementById("step-1");
const step2 = document.getElementById("step-2");
const step3 = document.getElementById("step-3");
const step4 = document.getElementById("step-4");

// 4. Situation Card Definitions
function getSituationCard(cardNum) {
    if (cardNum < 1 || cardNum > 36) return null;

    const effects = {}; // maps stock ID (1-9) -> price change modifier
    let desc = "";

    switch (cardNum) {
        case 1:
            effects[7] = 10;
            desc = "Automotive Boom: Three-for-one stock split rumored. United Auto price rises by $10.";
            break;
        case 2:
            effects[6] = 5;
            desc = "Transit Expansion: Tri-City Transport secures new Company location. Price rises by $5.";
            break;
        case 3:
            effects[3] = 3;
            effects[9] = 4;
            desc = "Bullish Investors: Buying wave raises the market, boosting Pioneer Mutual by $3 and Valley Power by $4.";
            break;
        case 4:
            effects[1] = 8;
            effects[2] = 5;
            effects[3] = 5;
            effects[7] = 7;
            desc = "Broad Economic Upswing: Growth Corp +$8, Metro Properties +$5, Pioneer Fund +$5, United Auto +$7.";
            break;
        case 5:
            effects[8] = 10;
            desc = "Research Breakthrough: Uranium Enterprises demonstrates highly economical power station designs. Three companies announce plans to adopt. Price jumps by $10.";
            break;
        case 6:
            effects[7] = -5;
            desc = "No Prizes for Second Place: United Auto competitor General Engines unveils new economical transmission column. Price drops by $5.";
            break;
        case 7:
            effects[1] = -10;
            desc = "Growth No More: Growth Corporation loses antitrust suit, forcing unexpected breakup. Price drops by $10.";
            break;
        case 8:
            effects[1] = -8;
            effects[2] = -5;
            effects[7] = -7;
            desc = "Widespread Profit-Taking: Growth Corp drops by $8, Metro Properties falls by $5, United Auto drops by $7.";
            break;
        case 9:
            effects[9] = 5;
            desc = "Coal Prices Drop: Valley Power passes savings from plants onto consumers. Black lung cases surge nationwide. Price rises by $5.";
            break;
        case 10:
            effects[6] = -5;
            desc = "Leadership Change-Up: Tri-City President involuntarily committed following incident abroad. Price drops by $5.";
            break;
        case 11:
            effects[8] = -25;
            desc = "Uranium Stockpiles Reach All-Time High: Congress ends price support on newly mined ore. Price falls by $25.";
            break;
        case 12:
            effects[7] = -15;
            desc = "Foreign Import Surge: United Auto loses market share due to plummeting demand for several signature models of 'compact' 4-ton vehicles. Price falls by $15.";
            break;
        case 13:
            effects[2] = 10;
            desc = "Prime Commercial Sales: Metro Properties boasts strong roster in City Council considerations for upcoming industrial fair. Price rises by $10.";
            break;
        case 14:
            effects[4] = -5;
            desc = "Community Transformed Into Urban Wasteland: Shady Brooks forced to lower rents after report finds most properties over 50% empty despite sky-high demand. Price drops by $5.";
            break;
        case 15:
            effects[8] = 10;
            desc = "Uranium Rush: Uranium Enterprises hires 'legendary' prospector Gus Chiggins, immediately discovers record-breaking deposit. Price rises by $10.";
            break;
        case 16:
            effects[6] = 10;
            desc = "Contract Windfall: In shocking sweep, Tri-City Transport lands three major contracts. Price rises by $10.";
            break;
        case 17:
            effects[7] = -15;
            desc = "Worker Strike: United Auto fails to reach deal with UAW, freezing production in eight plants. Price falls by $15.";
            break;
        case 18:
            effects[6] = 15;
            desc = "Mega Machines Contracts Tri-City Transport: Transit company lands decade-long contract with major industrial equipment distributor. Price rises by $15.";
            break;
        case 19:
            effects[4] = 5;
            desc = "Subdivisions Galore: Shady Brooks can hardly keep up with housing demand after new corporate campus opens nearby. Price rises by $5.";
            break;
        case 20:
            effects[3] = -8;
            effects[5] = 8;
            effects[8] = 5;
            desc = "War Looms: North Dakotan tensions over disputed Canadian border result in brief exchange. 0 casualties, 1 injured. Mixed activity on Wall Street. Stryker rises by $8 and Uranium rises by $5, but Pioneer Mutual Fund drops by $8.";
            break;
        case 21:
            effects[7] = 15;
            desc = "Auto Maker Expands: United Auto announces ambitious plan to increase domestic production by an estimated 30%. Price rises by $15.";
            break;
        case 22:
            effects[2] = -5;
            desc = "Metro Properties Sued: Annual Report shows incorrect Q4 net earnings, shedding light on deeper issues within accounting department. Price falls by $5.";
            break;
        case 23:
            effects[5] = -10;
            desc = "Land Dispute: Stryker Drilling halts construction on new mining rigs after lawsuit finds bogus deeds in Company holdings. Price drops by $10.";
            break;
        case 24:
            effects[1] = -10;
            desc = "Old Guard Retires: Growth Corporation loses several of its longest-serving executives. Analysts dubious of incoming leadership. Price drops by $10.";
            break;
        case 25:
            effects[2] = 5;
            desc = "Metro Tower Finds New Occupants: MoneyMakers LLC leases Metro Properties' crown jewel, Metro Tower, assuaging fears over the future of the building. Price rises by $5.";
            break;
        case 26:
            effects[1] = 8;
            desc = "Strong Performance Continues: Growth Corporation of America boasts yet another quarter of high profits and announces ambitious new R&D plans. Price rises by $8.";
            break;
        case 27:
            effects[9] = -14;
            desc = "Rate Hikes Rejected: Public Utility Commission rejects Valley Power's latest 'unconscionable' rate increases. Price drops by $14.";
            break;
        case 28:
            effects[9] = 5;
            desc = "New Plant Approved: Valley Power & Light will begin construction on new high-capacity reactor. When finished, plant estimated to quadruple local grid capacity. Price rises by $5.";
            break;
        case 29:
            effects[5] = -15;
            desc = "Environmental Protections Signed: Stryker Drilling's depletion allowance cut in half. Price drops by $15.";
            break;
        case 30:
            effects[5] = 17;
            desc = "Rexxon Deal Proposed: Oil conglomerate Rexxon offers to buy Stryker's aging assets for cash. High offering price raises eyebrows but pleases directors. Price jumps by $17.";
            break;
        case 31:
            effects[6] = -25;
            desc = "Uninsured Rail Hub Destroyed in Wildfire: Tri-City Transport route collapsed. Price drops by $25.";
            break;
        case 32:
            effects[2] = -10;
            desc = "City Planning Commission Gridlock: Metro Properties forced to delay major urban renewal program. Price drops by $10.";
            break;
        case 33:
            effects[1] = 10;
            desc = "Metallurgical Breakthrough: Growth Corporation announces 'revolutionary' metal forming process, applicable to both domestic and foreign metalworking operations. Price rises by $10.";
            break;
        case 34:
            effects[1] = -8;
            desc = "Boardroom Mayhem: Company founders publicly disagree on company direction and policy. In dramatic meeting, Seduardo Averin announces intentions to sell all company shares. Price drops by $8.";
            break;
        case 35:
            effects[7] = 10;
            desc = "Pivot to New Market: In surprise but savvy move, United Auto announces high-tech mini-cars to be sold next year in several trimlines. Price rises by $10.";
            break;
        case 36:
            effects[1] = 10;
            desc = "Board Announces Bonus Dividends: Growth Corporation Board announces additional around of dividends. President notes, 'After all, why shouldn't I?'. Price rises by $10. (Bonus $2 special dividend to all shareholders!)";
            break;
    }

    return { cardNum, desc, effects };
}

// 5. Game Operations & Logic Functions

// Initial setup listener
gameYearsInput.addEventListener("input", (e) => {
    yearsDisplay.textContent = `${e.target.value} Years`;
});

// Start game
startGameBtn.addEventListener("click", () => {
    totalYears = parseInt(gameYearsInput.value);
    currentYear = 1;
    gameActive = true;
    
    // Clear and duplicate starting values
    stocks = JSON.parse(JSON.stringify(initialStocks));
    
    setupPanel.style.display = "none";
    gameDashboard.style.display = "grid";
    resetGameBtn.style.display = "block";
    
    updateYearUI();
    renderChalkboard();
    updateDividendPayouts();
    resetRoundInputs();
    
    addLogEntry(`Session started. Game length: ${totalYears} years. All stocks initialized to $100.`, 'system-msg');
});

// Reset session
resetGameBtn.addEventListener("click", resetSession);
modalCloseBtn.addEventListener("click", () => {
    gameOverModal.style.display = "none";
    resetSession();
});

function resetSession() {
    gameActive = false;
    setupPanel.style.display = "flex";
    gameDashboard.style.display = "none";
    resetGameBtn.style.display = "none";
    logConsole.innerHTML = '<div class="log-entry system-msg">Session reset. Ready for new setup.</div>';
}

// Render dynamic table
function renderChalkboard() {
    // 1. Build Headers
    let headerHtml = `<th>Company</th><th>Div / 10 Sh.</th>`;
    for (let y = 1; y <= currentYear; y++) {
        headerHtml += `<th>Year ${y}</th>`;
    }
    tableHeaders.innerHTML = headerHtml;

    // 2. Build Body Rows
    let bodyHtml = "";
    stocks.forEach((stock) => {
        bodyHtml += `<tr>`;
        bodyHtml += `<td class="company-cell">${stock.name}</td>`;
        bodyHtml += `<td>${stock.dividend > 0 ? `$${stock.dividend}` : '—'}</td>`;
        
        for (let y = 0; y < currentYear; y++) {
            const displayVal = stock.displayPrices[y];
            const state = stock.states[y];
            
            let cellClass = "";
            let cellTitle = "";
            
            if (state === "split") {
                cellClass = "cell-split";
                cellTitle = "Stock Split: Halved value, double shares";
            } else if (state === "caution") {
                cellClass = "cell-caution";
                cellTitle = "No Dividend: Price is $50 or below";
            } else if (state === "reset") {
                cellClass = "cell-reset";
                cellTitle = "Stock Reset: Price fell to 0, reset to 100, shares forfeited";
            }
            
            bodyHtml += `<td class="${cellClass}" title="${cellTitle}">${displayVal}</td>`;
        }
        
        bodyHtml += `</tr>`;
    });
    tableBody.innerHTML = bodyHtml;
}

// Update Year display & step indicator active classes
function updateYearUI() {
    currentYearBadge.textContent = `Year ${currentYear} of ${totalYears}`;
}

// Update Dividend UI for active prices
function updateDividendPayouts() {
    dividendPayoutList.innerHTML = "";
    stocks.forEach((stock) => {
        const currentPrice = stock.prices[currentYear - 1];
        if (stock.dividend > 0) {
            const pays = currentPrice > 50;
            const item = document.createElement("div");
            item.className = "dividend-item";
            item.style.opacity = pays ? "1" : "0.55";
            item.innerHTML = `
                <span class="div-company">${stock.name}</span>
                <span class="div-amount" style="background: ${pays ? '' : 'rgba(239, 68, 68, 0.15)'}; color: ${pays ? '' : '#ef4444'}">
                    ${pays ? `$${stock.dividend}` : 'No Div ($50 or below)'}
                </span>
            `;
            dividendPayoutList.appendChild(item);
        }
    });
}

// System logging helper
function addLogEntry(text, className = "") {
    const entry = document.createElement("div");
    entry.className = `log-entry ${className}`;
    entry.innerText = text;
    logConsole.appendChild(entry);
    logConsole.scrollTop = logConsole.scrollHeight;
}

// Toggle logic for Bull/Bear
trendBullBtn.addEventListener("click", () => setMarketTrend('bull'));
trendBearBtn.addEventListener("click", () => setMarketTrend('bear'));

function setMarketTrend(trend) {
    marketTrend = trend;
    if (trend === 'bull') {
        trendBullBtn.classList.add("active");
        trendBearBtn.classList.remove("active");
    } else {
        trendBearBtn.classList.add("active");
        trendBullBtn.classList.remove("active");
    }
    step1.classList.add("completed");
    step1.classList.remove("active");
    step2.classList.add("active");
    validateTurnForm();
}

// Market roll 1d6
rollMarketBtn.addEventListener("click", () => {
    marketRollVal.textContent = "🎲";
    marketRollVal.classList.add("rolling");
    
    setTimeout(() => {
        const roll = Math.floor(Math.random() * 6) + 1;
        marketRollVal.textContent = roll;
        marketRollVal.classList.remove("rolling");
        
        const trend = (roll % 2 !== 0) ? 'bull' : 'bear';
        setMarketTrend(trend);
        addLogEntry(`🎲 Market Dice Roll: ${roll} -> ${trend.toUpperCase()} Market`, 'system-msg');
    }, 400);
});

// Price roll
priceRollInput.addEventListener("input", (e) => {
    const val = parseInt(e.target.value);
    if (val >= 2 && val <= 12) {
        priceRoll = val;
        step2.classList.add("completed");
        step2.classList.remove("active");
        step3.classList.add("active");
    } else {
        priceRoll = null;
    }
    validateTurnForm();
});

rollPriceBtn.addEventListener("click", () => {
    const dice = priceDicePreview.querySelectorAll(".die");
    dice[0].textContent = "🎲";
    dice[1].textContent = "🎲";
    dice[0].classList.add("rolling");
    dice[1].classList.add("rolling");
    
    setTimeout(() => {
        const d1 = Math.floor(Math.random() * 6) + 1;
        const d2 = Math.floor(Math.random() * 6) + 1;
        const sum = d1 + d2;
        
        dice[0].textContent = d1;
        dice[1].textContent = d2;
        dice[0].classList.remove("rolling");
        dice[1].classList.remove("rolling");
        
        priceRollInput.value = sum;
        priceRoll = sum;
        
        step2.classList.add("completed");
        step2.classList.remove("active");
        step3.classList.add("active");
        
        addLogEntry(`🎲 Price Shift Roll: ${d1} + ${d2} = ${sum}`, 'system-msg');
        validateTurnForm();
    }, 400);
});

// Draw Card / Input Card
eventCardInput.addEventListener("input", (e) => {
    const val = parseInt(e.target.value);
    if (val >= 1 && val <= 36) {
        setSituationCard(val);
    } else {
        clearSituationCard();
    }
});

drawCardBtn.addEventListener("click", () => {
    const cardNum = Math.floor(Math.random() * 36) + 1;
    eventCardInput.value = cardNum;
    setSituationCard(cardNum);
});

function setSituationCard(cardNum) {
    situationCard = getSituationCard(cardNum);
    
    cardDisplay.classList.remove("empty");
    cardDisplay.classList.add("active");
    cardDisplay.querySelector(".card-num-watermark").textContent = cardNum;
    cardDisplay.querySelector(".card-text").textContent = situationCard.desc;
    
    step3.classList.add("completed");
    step3.classList.remove("active");
    step4.classList.add("active");
    
    addLogEntry(`🃏 Drawn Situation Card #${cardNum}: ${situationCard.desc}`, 'news-msg');
    validateTurnForm();
}

function clearSituationCard() {
    situationCard = null;
    cardDisplay.classList.add("empty");
    cardDisplay.classList.remove("active");
    cardDisplay.querySelector(".card-num-watermark").textContent = "?";
    cardDisplay.querySelector(".card-text").textContent = "No card active. Click draw or enter a card number to see effects.";
    
    step3.classList.remove("completed");
    validateTurnForm();
}

// Enable/Disable advance button
function validateTurnForm() {
    const valid = (marketTrend !== null) && (priceRoll >= 2 && priceRoll <= 12) && (situationCard !== null);
    advanceYearBtn.disabled = !valid;
}

// Reset inputs for the next year
function resetRoundInputs() {
    marketTrend = null;
    priceRoll = null;
    situationCard = null;
    
    trendBullBtn.classList.remove("active");
    trendBearBtn.classList.remove("active");
    marketRollVal.textContent = "-";
    
    priceRollInput.value = "";
    const dice = priceDicePreview.querySelectorAll(".die");
    dice[0].textContent = "-";
    dice[1].textContent = "-";
    
    eventCardInput.value = "";
    clearSituationCard();
    
    step1.className = "phase-step active";
    step2.className = "phase-step";
    step3.className = "phase-step";
    step4.className = "phase-step";
    
    validateTurnForm();
}

// MAIN UPDATE CALCULATION
advanceYearBtn.addEventListener("click", () => {
    if (advanceYearBtn.disabled) return;
    
    const prevYearIndex = currentYear - 1;
    const lookupIndex = priceRoll - 2;
    const dataTable = (marketTrend === 'bull') ? bullMktData[lookupIndex] : bearMktData[lookupIndex];
    
    addLogEntry(`--- ADVANCING TO YEAR ${currentYear + 1} ---`, 'action-msg');
    
    // For each stock, calculate next price
    stocks.forEach((stock) => {
        const prevPrice = stock.prices[prevYearIndex];
        let baseChange = 0;
        
        if (stock.id > 0) {
            // Index 0 (Municipal Bonds) stays 100, no base price change
            baseChange = dataTable[stock.id - 1];
        }
        
        let cardChange = 0;
        if (situationCard.effects[stock.id]) {
            cardChange = situationCard.effects[stock.id];
        }
        
        const totalChange = baseChange + cardChange;
        
        // Target value calculations
        let newPrice = prevPrice + totalChange;
        let finalDisplayPrice = `${newPrice}`;
        let state = "normal";
        let actualNextPrice = newPrice;
        
        // Stock split logic: value >= 150
        if (newPrice >= 150) {
            state = "split";
            const splitPrice = Math.floor(newPrice / 2) + (newPrice % 2 !== 0 ? 1 : 0);
            finalDisplayPrice = `${newPrice} / ${splitPrice}`;
            actualNextPrice = splitPrice;
            addLogEntry(`🟢 ${stock.name} SPLITS! Price: ${newPrice} -> trading at ${splitPrice}. Shares doubled!`, 'system-msg');
        }
        // Stock reset logic: value <= 0
        else if (newPrice <= 0) {
            state = "reset";
            finalDisplayPrice = `0 (Reset to 100)`;
            actualNextPrice = 100;
            addLogEntry(`🔴 ${stock.name} RESETS! Price dropped below zero. Shares forfeited. Reset to 100.`, 'system-msg');
        }
        // Stock caution logic: value <= 50 (pays no dividends)
        else if (newPrice <= 50) {
            state = "caution";
            addLogEntry(`🟡 ${stock.name} price fell to $${newPrice}. Yields no dividends this turn.`, 'system-msg');
        }
        
        stock.prices.push(actualNextPrice);
        stock.displayPrices.push(finalDisplayPrice);
        stock.states.push(state);
    });
    
    currentYear++;
    updateYearUI();
    renderChalkboard();
    updateDividendPayouts();
    resetRoundInputs();
    
    // Check for game end
    if (currentYear > totalYears) {
        endSession();
    }
});

// Finalize Session / Show Modal
function endSession() {
    gameActive = false;
    advanceYearBtn.disabled = true;
    resetGameBtn.style.display = "none";
    
    // Fill game over modal with final prices
    finalPricesContainer.innerHTML = "";
    stocks.forEach((stock) => {
        const finalPrice = stock.prices[stocks[0].prices.length - 1];
        const item = document.createElement("div");
        item.className = "final-price-item";
        item.innerHTML = `
            <span class="final-company">${stock.name}</span>
            <span class="final-price">$${finalPrice}</span>
        `;
        finalPricesContainer.appendChild(item);
    });
    
    gameOverModal.style.display = "flex";
    addLogEntry(`*** Game Session Completed. Final chalkboard prices displayed. ***`, 'action-msg');
}

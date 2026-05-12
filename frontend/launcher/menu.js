// static://launcher/menu.js
// --- Menu Initialization ---

async function initializeMenu() {
    try {
        const result = await window.api.clearPlaybox('all');
        console.log(result.data?.message);
    } catch (err) {
        console.error('ClearPlaybox Error:', err);
    }

    const entries = [
        { name: "Game", config: "game.json", file: "game/index.html" },
        { name: "Credits", config: null, file: "licenses/licenses.html", modifier: "static" },
        { name: "Quit Game", config: null, file: null, modifier: "exit" },
        { name: "Manual", config: "procmon.json", file: "procmon/index.html" }
    ];

    // --- DOM references
    const body = document.body;

    // --- Header
    const title = document.createElement("h1");
    title.textContent = "Electron Playbox";
    body.appendChild(title);

    // --- Entry List Container
    const list = document.createElement("div");
    list.id = "entry-list";
    body.appendChild(list);

    // --- Populate the list
    for (const g of entries) {
        const btn = document.createElement("button");
        btn.className = "entry-button";
        btn.textContent = g.name;

        btn.addEventListener("click", async () => {
            try {
                // --- Handle Modifiers ---
                switch (g.modifier) {
                    case "exit":
                        await window.api.endSession();
                        return; // Stop execution here

                    case "static":
                        // Navigate directly without calling assemblePlaybox
                        const staticResult = await window.api.navigate(g.file, 'static');
                        if (!staticResult?.data?.success) {
                            createPopup(`Failed to navigate to ${g.name}`);
                        }
                        return; // Stop execution here
                }

                // --- Standard Logic (No modifier or unknown modifier) ---

                // Step 1: assemble playbox using config
                const result = await window.api.assemblePlaybox(g.config);
                if (!result?.data?.success) {
                    console.error("Assembly failed:", result?.data?.message);
                    createPopup(`Failed to assemble ${g.name}`);
                    return;
                }

                // Step 2: navigate to the entry file in the playbox
                const navResult = await window.api.navigate(`playbox/${g.file}`);
                if (!navResult?.data?.success) {
                    console.error("Navigation failed:", navResult?.data?.message);
                    createPopup(`Failed to navigate to ${g.name}`);
                }
            } catch (err) {
                console.error(err);
                createPopup("Unexpected error: " + err.message);
            }
        });
        list.appendChild(btn);
    }
}

function createPopup(message) {
    const popup = document.createElement('div');
    popup.classList.add('popup');

    const popupMessage = document.createElement('p');
    popupMessage.textContent = message;
    popup.appendChild(popupMessage);

    const okButton = document.createElement('button');
    okButton.textContent = 'OK';
    okButton.classList.add('popup-btn');
    okButton.addEventListener('click', () => popup.remove());

    popup.appendChild(okButton);
    document.body.appendChild(popup);
}


// --- Initialize menu on page load ---
async function startEngine(){
    const apps=await window.api.listApps();
    if(apps.data.count===0){
    await window.api.startApp("engine/main", "static");
    }

}
initializeMenu();
startEngine();

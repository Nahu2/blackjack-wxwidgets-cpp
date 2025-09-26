#include <wx/wx.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <random>
#include <ctime>
#include <algorithm>

//Card
struct Card {
    std::string suit;
    std::string rank;
    int value;
    std::string imagePath;//Path to the folder with all the card images
};

class BlackjackFrame : public wxFrame {
public:
    //Frame Constructor
    BlackjackFrame() : wxFrame(nullptr, wxID_ANY, "Nicolas' Casino Blackjack", wxDefaultPosition, wxSize(1100, 800)), chips(1000), currentBet(0) {
        srand(static_cast<unsigned>(time(0))); // Random Number Generator

        //Hex code of the background color
        this->SetBackgroundColour(wxColour("#339c5d"));

        //Font changer
        wxFont font(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
        wxFont fontmessageLabel(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

        //All labels/text displayed with corresponding fonts
        playerLabel = new wxStaticText(this, wxID_ANY, "Your Total = 0");
        dealerLabel = new wxStaticText(this, wxID_ANY, "Dealer's Total = ???");
        messageLabel = new wxStaticText(this, wxID_ANY, "Welcome to Blackjack!");
        chipsLabel = new wxStaticText(this, wxID_ANY, "Chips: 1000");
        betLabel = new wxStaticText(this, wxID_ANY, "Bet: 0");

        playerLabel->SetFont(font);
        dealerLabel->SetFont(font);
        messageLabel->SetFont(fontmessageLabel);//uses different font
        chipsLabel->SetFont(font);
        betLabel->SetFont(font);

        //Betting Buttons with corresponding font
        bet100Button = new wxButton(this, wxID_ANY, "Bet 100");
        bet250Button = new wxButton(this, wxID_ANY, "Bet 250");
        bet500Button = new wxButton(this, wxID_ANY, "Bet 500");
        betAllButton = new wxButton(this, wxID_ANY, "Bet All");

        bet100Button->SetFont(font);
        bet250Button->SetFont(font);
        bet500Button->SetFont(font);
        betAllButton->SetFont(font);

        //Buttons with corresponding font and dimentions
        hitButton = new wxButton(this, wxID_ANY, "Hit");
        standButton = new wxButton(this, wxID_ANY, "Stand");
        resetButton = new wxButton(this, wxID_ANY, "New Game");

        hitButton->SetFont(font);
        standButton->SetFont(font);
        resetButton->SetFont(font);

        wxSize buttonSize(150, 50);
        hitButton->SetMinSize(buttonSize);
        standButton->SetMinSize(buttonSize);
        resetButton->SetMinSize(buttonSize);

        //Event Bindings
        hitButton->Bind(wxEVT_BUTTON, &BlackjackFrame::OnHit, this);
        standButton->Bind(wxEVT_BUTTON, &BlackjackFrame::OnStand, this);
        resetButton->Bind(wxEVT_BUTTON, &BlackjackFrame::OnReset, this);

        bet100Button->Bind(wxEVT_BUTTON, &BlackjackFrame::OnBet100, this);
        bet250Button->Bind(wxEVT_BUTTON, &BlackjackFrame::OnBet250, this);
        bet500Button->Bind(wxEVT_BUTTON, &BlackjackFrame::OnBet500, this);
        betAllButton->Bind(wxEVT_BUTTON, &BlackjackFrame::OnBetAll, this);

        //Layout
        wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

        //Left section where the betting system is diplayed
        wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
        leftSizer->Add(chipsLabel, 0, wxALL | wxALIGN_LEFT, 10);
        leftSizer->Add(betLabel, 0, wxALL | wxALIGN_LEFT, 10);
        leftSizer->Add(bet100Button, 0, wxALL | wxALIGN_LEFT, 10);
        leftSizer->Add(bet250Button, 0, wxALL | wxALIGN_LEFT, 10);
        leftSizer->Add(bet500Button, 0, wxALL | wxALIGN_LEFT, 10);
        leftSizer->Add(betAllButton, 0, wxALL | wxALIGN_LEFT, 10);

        //Right section where the card game is displayed
        wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

        //Dealer display
        dealerCardsSizer = new wxBoxSizer(wxHORIZONTAL);//Sizer for dealer cards
        wxBoxSizer* dealerSectionSizer = new wxBoxSizer(wxVERTICAL);
        dealerSectionSizer->Add(dealerLabel, 0, wxALL | wxALIGN_LEFT, 10);
        dealerSectionSizer->Add(dealerCardsSizer, 0, wxALL | wxALIGN_LEFT, 10);

        //Separates the dealer and the player
        rightSizer->Add(dealerSectionSizer, 0, wxALIGN_CENTER_HORIZONTAL);
        rightSizer->AddSpacer(20);//Adds spaces between dealer and player

        //Player display
        playerCardsSizer = new wxBoxSizer(wxHORIZONTAL);//Sizer for player cards
        wxBoxSizer* playerSectionSizer = new wxBoxSizer(wxVERTICAL);
        playerSectionSizer->Add(playerLabel, 0, wxALL | wxALIGN_LEFT, 10);
        playerSectionSizer->Add(playerCardsSizer, 0, wxALL | wxALIGN_LEFT, 10);

        //Separates the player and the message
        rightSizer->Add(playerSectionSizer, 0, wxALIGN_CENTER_HORIZONTAL);
        rightSizer->AddSpacer(20);//Adds spaces between player and message

        //Message Label
        rightSizer->Add(messageLabel, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 20);

        //Button display
        wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        buttonSizer->Add(hitButton, 0, wxALL, 5);
        buttonSizer->Add(standButton, 0, wxALL, 5);
        buttonSizer->Add(resetButton, 0, wxALL, 5);

        //Add buttons
        rightSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 50);

        mainSizer->Add(leftSizer, 0, wxALL, 10);
        mainSizer->Add(rightSizer, 1, wxEXPAND);

        SetSizer(mainSizer);

        InitializeGame();
    }

private:
    wxStaticText* playerLabel;
    wxStaticText* dealerLabel;
    wxStaticText* messageLabel;
    wxStaticText* chipsLabel;
    wxStaticText* betLabel;
    wxButton* hitButton;
    wxButton* standButton;
    wxButton* resetButton;
    wxButton* bet100Button;
    wxButton* bet250Button;
    wxButton* bet500Button;
    wxButton* betAllButton;

    wxBoxSizer* playerCardsSizer;
    wxBoxSizer* dealerCardsSizer;

    std::vector<Card> deck;
    std::vector<Card> playerHand;
    std::vector<Card> dealerHand;
    int chips;
    int currentBet;

    void InitializeGame() {
        playerHand.clear();
        dealerHand.clear();
        deck = CreateDeck();

        DrawCard(playerHand);
        DrawCard(playerHand);
        DrawCard(dealerHand);
        DrawCard(dealerHand);

        UpdateUI(false);//Means that

        messageLabel->SetLabel("Place your bet!");
        hitButton->Enable(false);
        standButton->Enable(false);
        currentBet = 0;
    }

    //VERY IMPORTANT!!!!!!!!!!!!
    //In the FOR loop below, there should be the correct path in your local computer
    //The path that is there is for my computer, so it works correctly when I run it
    std::vector<Card> CreateDeck() {
        std::vector<Card> newDeck;
        std::vector<std::string> suits = {"Hearts", "Diamonds", "Clubs", "Spades"};
        std::vector<std::string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
        std::vector<int> values = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

        for (const auto& suit : suits) {
            for (size_t i = 0; i < ranks.size(); ++i) {
                std::string imagePath = "C:/Users/macha/OneDrive/Desktop/Nahu/Uni/Fall 2024/EEL 3370 (C++ Prog)/Nicolas' Casino BlackJack/CardImages/" + ranks[i] + "_of_" + suit + ".png";
                newDeck.push_back({suit, ranks[i], values[i], imagePath});
            }
        }
        std::random_device rd;
        std::default_random_engine rng(rd());
        std::shuffle(newDeck.begin(), newDeck.end(), rng);//Makes sure that the decks are random
        return newDeck;
    }

    void DrawCard(std::vector<Card>& hand) {
        if (!deck.empty()) {
            hand.push_back(deck.back());
            deck.pop_back();
        }
    }

    //This function will update the UI for the dealer after the player ended his turn
    void UpdateUI(bool revealDealer) {
        chipsLabel->SetLabel("Chips: " + std::to_string(chips));
        betLabel->SetLabel("Bet: " + std::to_string(currentBet));
        dealerCardsSizer->Clear(true);
        playerCardsSizer->Clear(true);
        for (size_t i = 0; i < dealerHand.size(); ++i) {
            if (!revealDealer && i > 0) {
                dealerCardsSizer->Add(new wxStaticBitmap(this, wxID_ANY, wxBitmap("C:/Users/macha/OneDrive/Desktop/Nahu/Uni/Fall 2024/EEL 3370 (C++ Prog)/Nicolas' Casino BlackJack/CardImages/back_of_card.png",
                                                                                   wxBITMAP_TYPE_PNG)), 0, wxALL, 5);
            } else {
                dealerCardsSizer->Add(new wxStaticBitmap(this, wxID_ANY, wxBitmap(dealerHand[i].imagePath, wxBITMAP_TYPE_PNG)), 0, wxALL, 5);
            }
        }
        for (const auto& card : playerHand) {
            playerCardsSizer->Add(new wxStaticBitmap(this, wxID_ANY, wxBitmap(card.imagePath, wxBITMAP_TYPE_PNG)), 0, wxALL, 5);
        }
        playerLabel->SetLabel("Your Total = " + std::to_string(CalculateHandValue(playerHand)));
        if (revealDealer) {
            dealerLabel->SetLabel("Dealer's Total = " + std::to_string(CalculateHandValue(dealerHand)));
        } else {
            dealerLabel->SetLabel("Dealer's Total = ???");
        }
        Layout();
    }

    //Placing a bet will take an amount out of your chips
    void PlaceBet(int amount) {
        if (chips >= amount) {
            currentBet += amount;
            chips -= amount;
            UpdateUI(false);
            hitButton->Enable(true);
            standButton->Enable(true);
            messageLabel->SetLabel("Your move!");
        } else {
            messageLabel->SetLabel("Not enough chips!");//Player will lose when they reach zero chips
        }
    }

    //these are how the betting buttons work
    void OnBet100(wxCommandEvent&) { PlaceBet(100); }
    void OnBet250(wxCommandEvent&) { PlaceBet(250); }
    void OnBet500(wxCommandEvent&) { PlaceBet(500); }
    void OnBetAll(wxCommandEvent&) { PlaceBet(chips); }

    //This function finds the total value of cards of either the dealerHand or playerHand and returns that number
    int CalculateHandValue(const std::vector<Card>& hand) {
        int value = 0;
        int aceCount = 0;
        for (const auto& card : hand) {
            value += card.value;
            if (card.rank == "A") {//I used the rank here instead of the value since the ace can hold a value of 11 or 1
                ++aceCount;
            }
        }
        while (value > 21 && aceCount > 0) {
            value -= 10;//minus 10 because it needs to go from 11 to 1 when the player goes over 21 total value
            --aceCount;
        }
        return value;
    }

    //this checks if after hitting, the player went over 21 and lost
    void OnHit(wxCommandEvent&) {
        DrawCard(playerHand);
        UpdateUI(false);
        if (CalculateHandValue(playerHand) > 21) {
            messageLabel->SetLabel("You busted! Dealer wins.");
            EndGame(false);
        }
    }

    void OnStand(wxCommandEvent&) {
        while (CalculateHandValue(dealerHand) < 17) {
            DrawCard(dealerHand);
        }
        UpdateUI(true);
        int playerValue = CalculateHandValue(playerHand);
        int dealerValue = CalculateHandValue(dealerHand);
        if (dealerValue > 21 || playerValue > dealerValue) {
            messageLabel->SetLabel("You win!");
            EndGame(true);
        } else if (playerValue < dealerValue) {
            messageLabel->SetLabel("Dealer wins.");
            EndGame(false);
        } else {
            messageLabel->SetLabel("It's a tie!");
            chips += currentBet;
            currentBet = 0;
            UpdateUI(true);
        }
    }

    void EndGame(bool playerWins) {
        hitButton->Enable(false);
        standButton->Enable(false);
        if (playerWins) {
            chips += currentBet * 2;
        }
        currentBet = 0;
        if (chips == 0) {
            wxMessageBox("You are out of chips! Game over.", "Game Over", wxOK | wxICON_INFORMATION);
            Close(true);//ends the whole application
        } else {
            wxMessageBox(playerWins ? "You win! Starting a new round." : "Dealer wins. Starting a new round.", "Round Over", wxOK | wxICON_INFORMATION);
            InitializeGame();
        }
    }

    void OnReset(wxCommandEvent&) {
        InitializeGame();
    }
};

//Main Application
class BlackjackApp : public wxApp {
public:
    virtual bool OnInit() {

        wxImage::AddHandler(new wxPNGHandler);//Helps handle .png files for the cards

        BlackjackFrame* frame = new BlackjackFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(BlackjackApp);

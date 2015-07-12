/*
 * simeple-leaderboard.js
 *
 * Simple Leaderboard + Click Game
 * Author      : Max Lee
 * Email       : hoso1312@gmail.com
 * Last Update : 12/07/15
 * */

// Global
PlayerList = new Mongo.Collection("players");
AutoCost = 500;
BombCost = 100;
BombVal = 50;

if (Meteor.isClient) {
  // Client side
  Meteor.subscribe("players");

  /* List Functions */
  Template.listplayer.helpers({
    shortDate: function() {
      return this.date.toLocaleString().split(" ")[0];  // Exclude time
    },

    selected: function() {
      return Session.get("selectedPlayer") == this.playerId 
    }
  });
  
  Template.listplayer.events({
    "click li": function() {
      Session.set("selectedPlayer", this.playerId);
      //var sel = Session.get("selectedPlayer");
      //console.log(sel)
      // Disabled logging 
    }
  });

  /* Body Functions */
  Template.body.helpers({
    players: function() {
      return PlayerList.find({}, {sort: {score: -1, player: 1}});
    },

    loggedin: function() {
      return Meteor.userId()
    }
  });

  /* Manually clicking */
  Template.clickgame.events({
    "click .playerclick": function() {
      Meteor.call('addScore', Meteor.userId());
      return false
    }
  });

  /* Items */
  Template.itemdisplay.helpers({
    autonum: function() {
      return PlayerList.findOne({playerId: Meteor.userId()}).autonum
    },

    autocost: function() {
      return AutoCost
    },

    bombcost: function() {
      return BombCost
    },

    bombamount: function() {
      return BombVal
    }
  });

  // Using items
  Template.itemdisplay.events({
    "click #buyAuto": function() {
      var playerdata = PlayerList.findOne({playerId: Meteor.userId()});

      if (playerdata.score >= AutoCost) {
        Meteor.call("addScore", playerdata.playerId, -AutoCost);
        Meteor.call("buyAuto", playerdata.playerId);
      return false
      }
    },

    "click #buyBomb": function() {
      var playerdata = PlayerList.findOne({playerId: Meteor.userId()});
      var sel = Session.get('selectedPlayer')

      if (playerdata.score >= BombCost && sel) {
        Meteor.call("addScore", playerdata.playerId, -BombCost);
        Meteor.call("addScore", sel, -BombVal);
      }
    }
  });

  // Autoclick ticker
  Template.itemdisplay.created = function() {
    this.handle = Meteor.setInterval((function() {
      if (Meteor.userId()){
        var playerdata = PlayerList.findOne({playerId: Meteor.userId()});
        if (playerdata.autonum && playerdata.autonum > 0) {
          Meteor.call('addScore', playerdata.playerId, playerdata.autonum);
        }
      }
    }), 1000);
  };

  // Account setup. No need Email address
  Accounts.ui.config({
    passwordSignupFields : "USERNAME_ONLY"
  });

};

if (Meteor.isServer) {
  // Server side
  Meteor.publish("players", function() {
    return PlayerList.find({});
  });
};

Meteor.methods({
  addScore: function(id, scr) {
    /* Add score function. id = player(user) id. scr = score to add.
     * Can pass negative value to scr.
     * Default value of scr is 1.*/
    scr = typeof scr !== 'undefined' ? scr : 1;
    if (PlayerList.findOne({playerId: id})) {
      PlayerList.update({playerId: id}, {$inc: {score: scr}}); // update exsisting using scr
    } else {
      // create new document
      PlayerList.insert({playerId: Meteor.userId(), player: Meteor.user().username, score: 1, date: new Date(), autonum: 0});
    }
  },

  buyAuto: function(id) {
    /* buy autoclick function, id = player(user) id
     * increases autoclick by 1.*/
    PlayerList.update({playerId: id}, {$inc: {autonum: 1}})
  }
});


PlayerList = new Mongo.Collection("players"); // global

if (Meteor.isClient) {
  // Client side
  Meteor.subscribe("players");

  Template.listplayer.helpers({
    player: function() {
      return this.player
    },

    score: function() {
      return this.score
    },

    date: function() {
      return this.date;
    }
  });

  Template.body.helpers({
    players: function() {
      return PlayerList.find({}, {sort: {score: -1}});
    },

    loggedin: function() {
      return Meteor.userId()
    }
  });

  Template.clickgame.events({
    "click .playerclick": function() {
      Meteor.call('addScore', Meteor.userId());
      return false
    }
  });

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

  addScore: function(id) {
    if (PlayerList.findOne({playerId: id})) {
      PlayerList.update({playerId: id}, {$inc: {score: 1}});
      //TODO: use findandupdate method
    } else {
      PlayerList.insert({playerId: Meteor.userId(), player: Meteor.user().username, score: 1, date: new Date()});
    }
  }

});


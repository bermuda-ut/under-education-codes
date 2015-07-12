/*
 * simple-todos.js
 * Max Lee
 * hoso1312@gmail.com
 * 11/07/15
 * Created following the offical Meteor tutorial
 * */
Tasks = new Mongo.Collection("tasks");

if (Meteor.isClient){
  // Only runs on the client
  Meteor.subscribe("tasks");


  Template.task.helpers({
    isOwner: function() {
      return this.owner == Meteor.userId();
    },

    shortDate: function() {
      longDate = this.createdAt
      return String(longDate.getDate()) + "/" + String(longDate.getMonth() + 1)
    }

  });


  Template.task.events({
    "click .toggle-public": function () {
      Meteor.call("setPublic", this._id, !this.isPublic);
    }
  });


  Template.body.helpers({
    tasks: function () {
      if (Session.get("hideCompleted")) {
        // If hide is chekced, filter
        return Tasks.find({checked: {$ne: true}}, {sort: {createdAt: -1}});
      } else {
        // Otherwise return all of the tasks
        return Tasks.find({}, {sort: {createdAt: -1}});
      }
    },

    hideCompleted: function() {
      // This ensures when label is clicked and triggers the event, the checkbox gets ticked.
      return Session.get("hideCompleted")
    },

    incompleteCount: function() {
      return Tasks.find({checked: {$ne: true}, owner: Meteor.userId()}).count();
    }
  });


  Template.body.events({
    "submit .new-task": function (event) {
      // This function is called when new task form is submitted
      var text = event.target.text.value;

      if (text != ""){ // avoid "" submission
        Meteor.call("addTask", text);
      }

      event.target.text.value = ""; // Clear form

      return false; // Prevent default form submit
    },

    "click .toggle-checked": function () {
      Meteor.call("setChecked", this._id, !this.checked);
    },

    "click .delete": function () {
      Meteor.call("deleteTask", this._id);
    },

    "change .hide-completed input": function (event) {
      Session.set("hideCompleted", event.target.checked);
    }
  });


  Accounts.ui.config({
    passwordSignupFields: "USERNAME_ONLY"
    // use username only instead of email address
  });


}


Meteor.methods({

  addTask: function(task_name) {
    // make sure user is logged in before inserting a task
    if (! Meteor.userId()){
      throw new Meteor.error("not authorized");
    }

    Tasks.insert({
      text: task_name,
      createdAt: new Date(),
      owner: Meteor.userId(),
      username: Meteor.user().username
    });
  },


  deleteTask: function (taskId) {
    var task = Tasks.findOne(taskId);
    if (task.isPublic && task.owner !== Meteor.userId()) {
      // only task creator can delete
      //throw new Meteor.error("Only task creator can delete the task");
      if (Meteor.isClient){
        alert("Only task creator can delete the task");
      } else {
        console.log(String(Meteor.user().username) + " tried to delete task ID:" + taskId + ", owned by " + String(task.username));
      }
    } else {
    Tasks.remove(taskId);
    }
  },

  setChecked: function (taskId, setChecked) {
    var task = Tasks.findOne(taskId);
    if (task.isPublic && task.owner !== Meteor.userId()) {
      // only task creator can delete
      if (Meteor.isClient){
        alert("Only task creator can complete the task");
      } else {
        console.log(String(Meteor.user().username) + " tried to complete task ID:" + taskId + ", owned by " + String(task.username));
      }
    } else {
    Tasks.update(taskId, {$set: {checked: setChecked} });
    }
  },

  setPublic: function (taskId, setToPublic) {
    var task = Tasks.findOne(taskId);

    if (task.owner !== Meteor.userId()) {
      throw new Meteor.error("unauthorized access");
    }

    Tasks.update(taskId, {$set: {isPublic: setToPublic}});
  }

});


if (Meteor.isServer) {
  Meteor.publish("tasks", function() {
    return Tasks.find({
      $or: [
        {owner: this.userId},
        {isPublic: true}
    ]
    });
  });
};

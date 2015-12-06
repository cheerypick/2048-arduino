/**
 * Created by Ekaterina Orlova on 05/12/15.
 */
var app = angular.module('app', ['ngRoute', 'ui.bootstrap.datetimepicker']);

// configure our routes
app.config(function ($routeProvider) {

    console.log($routeProvider);
    $routeProvider

        // route for the home page
        .when('/app', {
            templateUrl: 'views/home.html',
            controller: 'mainController'
        })

        .when('/app/events', {
            templateUrl: 'views/events-list.html',
            controller: 'EventListController'
        })
        .when('/app/create-event', {
            templateUrl: 'views/create-event.html',
            controller: 'CreateEventController'
        })

        // route for the about page
        .when('/app/events/:id', {
            templateUrl: 'views/event-details.html',
            controller: 'EventDetailsController'
        }).
        otherwise({
            redirectTo: "/app"
        });
});

app.controller('mainController', function ($scope) {

});
// create the controller and inject Angular's $scope
app.controller('EventListController', ['$scope', '$http', function ($scope, $http) {


    $scope.getEvents = function () {
        $http.get('services/events/').success(function (response) {
            $scope.events = response;
        });
    };

    $scope.createEvent = function () {
        $http.post('services/events/').success(function (response) {
            $scope.events = response;
        });
    };


    $scope.getEvents()
}]);

app.controller('EventDetailsController', ['$scope', '$http', '$routeParams', function ($scope, $http, $routeParams) {

    var id = $routeParams.id;


    $scope.getEvents = function () {
        $http.get('services/events/' + id).success(function (response) {
            $scope.event = response;
        });
    };

    $scope.getEvents()
}]);

app.controller('CreateEventController', ['$scope', '$http', '$routeParams', function ($scope, $http, $routeParams) {

    var event = {
        startDateTime: "01-02-2014",
        endDateTime: "01-02-2014",
        subject: 2,
        title: 'Julebord'
    }

    console.log($scope.event);

    console.log("Create!");
    $scope.createEvent = function () {
        $http.post('services/events/', $scope.event).success(function (response) {
            console.log("Created!");
        });
    };

}]);
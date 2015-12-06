angular.module('testApp').controller('EventDetailsController', ['$scope', '$http', function ($scope, $http) {



    $scope.getEvents = function () {
        $http.get('services/events/').success(function (response) {
            $scope.events = response;
        });
    };

    $scope.createUser = function () {
        $http.post('services/events/').success(function (response) {
            $scope.events = response;
        });
    };


    $scope.getEvents()
}]);


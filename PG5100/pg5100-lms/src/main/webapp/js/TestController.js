angular.module('testApp').controller('TestController', ['$scope', '$http', 'greetingService', function ($scope, $http, greetingService) {
    $scope.greeting = function () {
        return "Hello World!";

    };

    $scope.greetingService = function () {
        $scope.result = greetingService.greeting($scope.name);
    };

    $scope.getBook = function () {
        $http.get('services/events/1').success(function (data) {
            $scope.result = data.title;
        });
    }
}]);


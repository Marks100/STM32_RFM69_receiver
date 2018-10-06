# todo add Story Name and description
Feature: Concise User Story name that summarizes the User Story
  As a <type of user>, I want <some goal> so that <some reason>
  + any additional relevant information

# todo add any steps (setUp) that should be done before all scenarios / test cases.
  Background:
    Given I have a precondition that is applicable to all test cases

# todo for each Test Case / Scenario add unique Scenario name and describe test steps in terms of Given, When Then format
  Scenario: Concise description of Scenario 1
    Given I have some precondition
      And another precondition
    When I trigger some event
      And I trigger another event
    Then I expect to see the expected result
      And another expected result

# todo add additional Test Cases / Scenarios until all acceptance criteria has been tested
  Scenario: Concise description of Scenario 2
    Given some precondition
    And some other precondition
    When some action by the user
    And some other action
    And yet another action
    Then some testable outcome is achieved
    And something else we can check happens too

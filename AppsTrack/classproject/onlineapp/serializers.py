from rest_framework import serializers
from onlineapp.models import *
from django.contrib.auth.models import *


class CollegeSerializer(serializers.ModelSerializer):
    class Meta:
        model = College
        fields = ('id', 'name', 'location', 'acronym', 'contact')


class MockTestSerializer(serializers.ModelSerializer):
    class Meta:
        model = MockTest1
        fields = ('id', 'problem1', 'problem2', 'problem3', 'problem4')


class StudentSerializer(serializers.ModelSerializer):
    mocktest1 = MockTestSerializer(required=True)

    class Meta:
        model = Student
        fields = ('id', 'name', 'dob', 'email', 'db_folder', 'dropped_out', 'mocktest1')
        depth = 1

    def create(self, validated_data):
        mocktest_data = validated_data.pop('mocktest1')
        college = College.objects.get(pk=self.context.get('college_id'))
        student = Student.objects.create(**validated_data, college=college)
        mocktest_data["total"] = mocktest_data["problem1"] \
                                 + mocktest_data["problem2"] \
                                 + mocktest_data["problem3"] \
                                 + mocktest_data["problem4"]
        MockTest1.objects.create(**mocktest_data, student=student)
        return student

    def update(self, instance, validated_data):
        instance.name = validated_data.get("name", instance.name)
        instance.dob = validated_data.get("dob", instance.dob)
        instance.email = validated_data.get("email", instance.email)
        instance.db_folder = validated_data.get("db_folder", instance.db_folder)
        instance.dropped_out = validated_data.get("dropped_out", instance.dropped_out)

        instance.mocktest1.problem1 = validated_data.get("mocktest1", instance.mocktest1).get("problem1",
                                                                                              instance.mocktest1.problem1)
        instance.mocktest1.problem2 = validated_data.get("mocktest1", instance.mocktest1).get("problem1",
                                                                                              instance.mocktest1.problem2)
        instance.mocktest1.problem3 = validated_data.get("mocktest1", instance.mocktest1).get("problem3",
                                                                                              instance.mocktest1.problem3)
        instance.mocktest1.problem4 = validated_data.get("mocktest1", instance.mocktest1).get("problem4",
                                                                                              instance.mocktest1.problem4)

        instance.mocktest1.total = instance.mocktest1.problem1 \
                                   + instance.mocktest1.problem2 \
                                   + instance.mocktest1.problem3 \
                                   + instance.mocktest1.problem4

        instance.save()
        instance.mocktest1.save()
        return instance

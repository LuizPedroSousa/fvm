#pragma once
class BaseException
{
public:
  char *message;
  BaseException(char *message);
  BaseException();
};
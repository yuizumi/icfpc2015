#ifndef MACROS_H_
#define MACROS_H_

#define DISALLOW_COPY_AND_ASSIGN(TypeName)      \
    void operator=(const TypeName&);            \
    TypeName(const TypeName&);

#endif  // MACROS_H_
